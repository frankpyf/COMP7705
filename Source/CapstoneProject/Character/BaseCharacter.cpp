// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "CapstoneProject/CapstoneProject.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
}

void ABaseCharacter::OnHit()
{
	
}

void ABaseCharacter::OnDeath()
{
	// Multicast Death (Play sound and animation, etc.) 
	HandleDeath();
}

void ABaseCharacter::HandleDeath_Implementation()
{
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	// TODO: Add Death Impluse

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

FVector ABaseCharacter::GetAttackSocketLocation_Implementation()
{
	return GetMesh()->GetSocketLocation("SpellLocation");
}

void ABaseCharacter::GiveAbility(TSubclassOf<UGameplayAbility> NewAbility)
{
	if(!HasAuthority() || !AbilitySystemComponent || !IsValid(NewAbility))
		return;
	const FGameplayAbilitySpec Spec(NewAbility);
	AbilitySystemComponent->GiveAbility(Spec);
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

