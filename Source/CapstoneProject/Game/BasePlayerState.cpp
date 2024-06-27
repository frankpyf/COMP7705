// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerState.h"
#include "AbilitySystemComponent.h"
#include "CapstoneProject/GAS/CharacterBaseAttributeSet.h"

ABasePlayerState::ABasePlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));
	BaseAttributeSet = CreateDefaultSubobject<UCharacterBaseAttributeSet>(TEXT("BaseAttributeSet"));
}

void ABasePlayerState::BeginPlay()
{
	Super::BeginPlay();
	
	// Bind Functions to Ability System
	if(IsValid(AbilitySystemComponent))
	{
		BaseAttributeSet = AbilitySystemComponent->GetSet<UCharacterBaseAttributeSet>(); 
	}
}

const UCharacterBaseAttributeSet* ABasePlayerState::GetBaseAttributeSet() const
{
	return BaseAttributeSet;
}
