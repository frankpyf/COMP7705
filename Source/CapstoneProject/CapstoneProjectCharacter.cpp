// Copyright Epic Games, Inc. All Rights Reserved.

#include "CapstoneProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "CapstoneProjectPlayerController.h"
#include "CharacterBaseAttributeSet.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "GameFramework/SpringArmComponent.h"

ACapstoneProjectCharacter::ACapstoneProjectCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));
	BaseAttributeSet = CreateDefaultSubobject<UCharacterBaseAttributeSet>(TEXT("BaseAttributeSet"));
}

void ACapstoneProjectCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Add Input Mapping Context
	if(auto PC = CastChecked<APlayerController>(GetController()))
	{
		if (auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Bind Functions to Ability System
	if(IsValid(AbilitySystem))
	{
		BaseAttributeSet = AbilitySystem->GetSet<UCharacterBaseAttributeSet>();
		AbilitySystem->GetGameplayAttributeValueChangeDelegate(UCharacterBaseAttributeSet::GetHealthAttribute())
		.AddUObject(this, &ACapstoneProjectCharacter::OnHealthChanged);

		AbilitySystem->GetGameplayAttributeValueChangeDelegate(UCharacterBaseAttributeSet::GetSpeedAttribute())
		.AddUObject(this, &ACapstoneProjectCharacter::OnMoveSpeedChanged);
	}
}


void ACapstoneProjectCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void ACapstoneProjectCharacter::Destroyed()
{
	Super::Destroyed();
}

void ACapstoneProjectCharacter::GrantAbility(TSubclassOf<UGameplayAbility> Ability, int32 Level, int32 InputCode)
{
	if(!IsValid(AbilitySystem) || !IsValid(Ability) || !HasAuthority())
		return;
	const FGameplayAbilitySpec AbilitySpec{Ability, Level, InputCode};
	AbilitySystem->GiveAbility(AbilitySpec);
}

void ACapstoneProjectCharacter::GrantAbilities(const TArray<TSubclassOf<UGameplayAbility>> Abilities)
{
	if(!IsValid(AbilitySystem) || !HasAuthority())
		return;
	for(const auto& Ability : Abilities)
	{
		if(!IsValid(Ability))
			continue;
		const FGameplayAbilitySpec AbilitySpec{Ability};
		AbilitySystem->GiveAbility(AbilitySpec);
	}
}

float ACapstoneProjectCharacter::GetHealth() const
{
	if(IsValid(BaseAttributeSet))
	{
		return BaseAttributeSet->GetHealth();
	}
	return -1.f;
}

float ACapstoneProjectCharacter::GetMaxHealth() const
{
	if(IsValid(BaseAttributeSet))
	{
		return BaseAttributeSet->GetMaxHealth();
	}
	return -1.f;
}

void ACapstoneProjectCharacter::OnHealthChanged(const FOnAttributeChangeData& Data) const
{
	HealthChanged.Broadcast(Data.NewValue);
	// Player Died
	if(Data.NewValue == 0)
	{
		OnDead();
	}
}

void ACapstoneProjectCharacter::OnMoveSpeedChanged(const FOnAttributeChangeData& Data) const
{
	if(const auto MovementComp = CastChecked<UCharacterMovementComponent>(GetMovementComponent()))
	{
		MovementComp->MaxWalkSpeed = Data.NewValue;
	}
}

void ACapstoneProjectCharacter::OnDead() const
{
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->WakeAllRigidBodies();
	GetMesh()->bBlendPhysics = true;
	
	GetCharacterMovement()->DisableMovement();
}

void ACapstoneProjectCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D& MovementVector = Value.Get<FVector2D>();
		
	const FRotator& Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
	// get right vector 
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// add movement 
	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void ACapstoneProjectCharacter::RotateCamera()
{
	if(auto SpringArm = GetComponentByClass<USpringArmComponent>())
	{
		SpringArm->AddRelativeRotation(FRotator(0.f, 90.f, 0.f));
	}
}

void ACapstoneProjectCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if(MoveAction)
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACapstoneProjectCharacter::Move);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

