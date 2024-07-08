// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroCharacter.h"

#include "AbilitySystemComponent.h"
#include "EnemyCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "CapstoneProject/GAs/CharacterBaseAttributeSet.h"
#include "CapstoneProject/Game/BasePlayerState.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayEffectTypes.h"
#include "CapstoneProject/Components/InteractionComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include <Kismet/GameplayStatics.h>
#include <CapstoneProject/Game/CapstoneProjectGameMode.h>
#include "CapstoneProject/Save/LoadScreenSaveGame.h"

AHeroCharacter::AHeroCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	
	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	
	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>("InteractionComponent");
}

void AHeroCharacter::SetComboNotify(UComboWindow* InNotify)
{
	ComboNotify = InNotify;
}

void AHeroCharacter::SetCanCombo(bool NewFlag)
{
	bComboWindow = NewFlag;
}

void AHeroCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//Add Input Mapping Context
	if(auto PC = GetController<APlayerController>())
	{
		if (auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Bind Functions to Ability System
	if(IsValid(AbilitySystemComponent))
	{
		BaseAttributeSet = AbilitySystemComponent->GetSet<UCharacterBaseAttributeSet>();
		
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UCharacterBaseAttributeSet::GetHealthAttribute())
		.AddLambda([this](const FOnAttributeChangeData& Data)
		{
			HealthChanged.Broadcast(Data.NewValue);
		});
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UCharacterBaseAttributeSet::GetMaxHealthAttribute())
		.AddLambda([this](const FOnAttributeChangeData& Data)
		{
			MaxHealthChanged.Broadcast(Data.NewValue);
		});
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UCharacterBaseAttributeSet::GetStaminaAttribute())
		.AddLambda([this](const FOnAttributeChangeData& Data)
		{
			if(Data.NewValue < 25)
			{
				bExhausted = true;
				GetCharacterMovement()->MaxWalkSpeed = 400.f;
			}
			else if(Data.NewValue == 100)
			{
				AbilitySystemComponent->RemoveActiveGameplayEffect(ActiveStaminaRecoverEffect);
				if(bExhausted)
				{
					bExhausted = false;
					GetCharacterMovement()->MaxWalkSpeed = bLockOnEnemy ? 300.f : 600.f;
				}
			}
			if(Data.NewValue < Data.OldValue)
			{
				auto SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(StaminaRecoverEffect, 1.0, AbilitySystemComponent->MakeEffectContext());
				ActiveStaminaRecoverEffect = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
			
			// Broadcast
			StaminaChanged.Broadcast(Data.NewValue);
		});
	}
}

void AHeroCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DoLockOn(DeltaTime);
}

void AHeroCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Set Actor Info for the Sever
	InitAbilityActorInfo();

	// Give Default Abilities
	for(const auto Ability : DefaultAbilities)
	{
		GiveAbility(Ability);
	}
}

void AHeroCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Set Actor Info for the Client
	InitAbilityActorInfo();
}


void AHeroCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D& MovementVector = Value.Get<FVector2D>();
		
	const FRotator& Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector ForwardDirection = bLockOnEnemy ? GetActorForwardVector() : FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
	// get right vector 
	const FVector RightDirection = bLockOnEnemy ? GetActorRightVector() : FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// add movement 
	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void AHeroCharacter::TryLockOn()
{
	// Get Nearest Enemy
	TArray<FHitResult> Hits;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	UKismetSystemLibrary::SphereTraceMultiForObjects(this,
		GetActorLocation() + FVector(0, 0, 100),
		GetActorLocation(),
		LockRange,
		ObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		Hits,
		true);
	float MinDist = TNumericLimits<float>::Max();
	for(const auto Hit : Hits)
	{
		if(auto Enemy = Cast<AEnemyCharacter>(Hit.GetActor()))
		{
			const FVector EnemyLocation = Enemy->GetActorLocation();
			const float Dist = FVector::Dist(GetActorLocation(), EnemyLocation);
			if(Dist < MinDist)
			{
				FocusedEnemy = Enemy;
				MinDist = Dist;
			}
		}
	}
	if(FocusedEnemy)
	{
		bLockOnEnemy = true;
		WantToStrafe();
	}
	else
	{
		bLockOnEnemy = false;
		StopStrafing();
	}
}

void AHeroCharacter::StopLocking()
{
	bLockOnEnemy = false;
	FocusedEnemy = nullptr;
	StopStrafing();
}

void AHeroCharacter::DoLockOn(float DeltaTime)
{
	if(!bLockOnEnemy)
		return;
	if(!IsValid(FocusedEnemy))
	{
		StopLocking();
		return;
	}

	const FVector EnemyLocation = FocusedEnemy->GetActorLocation();
	const FVector HeroLocation = GetActorLocation();
	const FVector TargetLocation(EnemyLocation.X, EnemyLocation.Y, HeroLocation.Z);

	// Check Focused Enemy Still in Range
	if(FVector::Dist(HeroLocation, EnemyLocation) > LockRange)
	{
		FocusedEnemy = nullptr;
		// if out of range try get one
		TryLockOn();
		if(!FocusedEnemy)
		{
			StopLocking();
			return;
		}
	}

	// Lock On Enemy
	const auto TurnRotator = UKismetMathLibrary::FindLookAtRotation(HeroLocation, TargetLocation);
	
	SetActorRotation(TurnRotator);
}

void AHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if(MoveAction)
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AHeroCharacter::Move);
	}
}

void AHeroCharacter::InitAbilityActorInfo()
{
	if(auto HeroPlayerState = GetPlayerState<ABasePlayerState>())
	{
		auto PlayerAbilitySystem = HeroPlayerState->GetAbilitySystemComponent();
		// it must have a valid ASC
		check(PlayerAbilitySystem);
		PlayerAbilitySystem->InitAbilityActorInfo(HeroPlayerState, this);

		AbilitySystemComponent = PlayerAbilitySystem;
		BaseAttributeSet = HeroPlayerState->GetBaseAttributeSet();
	}
}

void AHeroCharacter::WantToStrafe()
{
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
}

void AHeroCharacter::StopStrafing()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->MaxWalkSpeed = bExhausted ? 400.f : 600.f;
}

void AHeroCharacter::SaveProgress_Implementation(const FName& CheckpointTag)
{
	ACapstoneProjectGameMode* CPGameMode = Cast<ACapstoneProjectGameMode>(UGameplayStatics::GetGameMode(this));
	if (CPGameMode)
	{
		ULoadScreenSaveGame* SaveData = CPGameMode->RetrieveInGameSaveData();
		if (SaveData == nullptr) return;

		SaveData->PlayerStartTag = CheckpointTag;

		if (ABasePlayerState* PlayerState = Cast<ABasePlayerState>(GetPlayerState()))
		{
			//SaveData->PlayerLevel = PlayerState->GetPlayerLevel();
			//SaveData->XP = PlayerState->GetXP();
			//SaveData->AttributePoints = PlayerState->GetAttributePoints();
			//SaveData->SpellPoints = PlayerState->GetSpellPoints();
		}
		SaveData->Health = UCharacterBaseAttributeSet::GetHealthAttribute().GetNumericValue(GetAttributeSet());
		SaveData->MaxHealth = UCharacterBaseAttributeSet::GetMaxHealthAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Stamina = UCharacterBaseAttributeSet::GetStaminaAttribute().GetNumericValue(GetAttributeSet());
		SaveData->MaxStamina = UCharacterBaseAttributeSet::GetMaxStaminaAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Mana = UCharacterBaseAttributeSet::GetManaAttribute().GetNumericValue(GetAttributeSet());
		SaveData->MaxMana = UCharacterBaseAttributeSet::GetMaxManaAttribute().GetNumericValue(GetAttributeSet());
		SaveData->IncomingDamage = UCharacterBaseAttributeSet::GetIncomingDamageAttribute().GetNumericValue(GetAttributeSet());

		SaveData->bFirstTimeLoadIn = false;

		if (!HasAuthority()) return;

		CPGameMode->SaveInGameProgressData(SaveData);
	}
}