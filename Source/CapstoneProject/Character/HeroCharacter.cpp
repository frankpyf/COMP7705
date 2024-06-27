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
#include "GameFramework/SpringArmComponent.h"
#include "GameplayEffectTypes.h"
#include "CapstoneProject/Components/InteractionComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

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
		.AddUObject(this, &AHeroCharacter::OnHealthChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UCharacterBaseAttributeSet::GetMaxHealthAttribute())
		.AddUObject(this, &AHeroCharacter::OnMaxHealthChanged);
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

	const FRotator& CameraRotation = GetComponentByClass<USpringArmComponent>()->GetRelativeRotation();

	// get forward vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
	// get right vector 
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// add movement 
	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void AHeroCharacter::RotateCamera()
{
	if (auto SpringArm = GetComponentByClass<USpringArmComponent>())
	{
		SpringArm->AddRelativeRotation(FRotator(0.f, 90.f, 0.f));
	}
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
	if(!FocusedEnemy)
	{
		StopLocking();
		return;
	}

	const FVector EnemyLocation = FocusedEnemy->GetActorLocation();
	const FVector HeroLocation = GetActorLocation();

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
	const auto TurnRotator = UKismetMathLibrary::FindLookAtRotation(HeroLocation, EnemyLocation);
	
	SetActorRotation(TurnRotator);
}

void AHeroCharacter::OnHealthChanged(const FOnAttributeChangeData& Health) const
{
	HealthChanged.Broadcast(Health.NewValue);
}

void AHeroCharacter::OnMaxHealthChanged(const FOnAttributeChangeData& MaxHealth) const
{
	MaxHealthChanged.Broadcast(MaxHealth.NewValue);
}

void AHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if(MoveAction)
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AHeroCharacter::Move);

		if(RotateCameraAction)
			EnhancedInputComponent->BindAction(RotateCameraAction, ETriggerEvent::Triggered, this, &AHeroCharacter::RotateCamera);
	}
	else
	{
		// UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
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

void AHeroCharacter::WantToStrafe() const
{
	// TODO: Modify Speed
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

void AHeroCharacter::StopStrafing() const
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
}
