// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "AbilitySystemComponent.h"
#include "CapstoneProject/CharacterBaseAttributeSet.h"

AEnemyCharacter::AEnemyCharacter()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));
	BaseAttributeSet = CreateDefaultSubobject<UCharacterBaseAttributeSet>(TEXT("BaseAttributeSet"));
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// Bind Functions to Ability System
	if(IsValid(AbilitySystemComponent))
	{
		BaseAttributeSet = AbilitySystemComponent->GetSet<UCharacterBaseAttributeSet>();
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}
