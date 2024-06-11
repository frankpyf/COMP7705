// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBaseAttributeSet.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"

void UCharacterBaseAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if(Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	if(Data.EvaluatedData.Attribute == GetEnduranceAttribute())
	{
		SetEndurance(FMath::Clamp(GetEndurance(), 0.f, GetMaxEndurance()));
	}
	if(Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}
}
