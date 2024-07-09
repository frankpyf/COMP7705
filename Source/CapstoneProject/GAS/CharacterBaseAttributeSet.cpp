// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBaseAttributeSet.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "CapstoneProject/Interfaces/CombatInterface.h"

void UCharacterBaseAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if(Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	if(Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(FMath::Clamp(GetStamina(), 0.f, GetMaxStamina()));
	}
	if(Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}

	if(Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		const float LocalIncomingDamage = GetIncomingDamage();
		SetIncomingDamage(0.f);
		if(LocalIncomingDamage > 0.f)
		{
			const float NewHealth = GetHealth() - LocalIncomingDamage;
			SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));
			
			if(Data.Target.GetAvatarActor()->Implements<UCombatInterface>())
			{
				const bool bFatal = NewHealth <= 0.f;
				ICombatInterface::Execute_OnHit(Data.Target.GetAvatarActor());
				if(bFatal)
				{
					ICombatInterface::Execute_OnDeath(Data.Target.GetAvatarActor());
				}
			}
		}
	}
	
}
