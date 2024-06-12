// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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

