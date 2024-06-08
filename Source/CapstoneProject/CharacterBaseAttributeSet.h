// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define ATTRIBUTE_ACCESSORS(Class, Property) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(Property) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(Property) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(Property) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(Class, Property);

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "CharacterBaseAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONEPROJECT_API UCharacterBaseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UCharacterBaseAttributeSet, Health)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UCharacterBaseAttributeSet, MaxHealth)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character")
	FGameplayAttributeData Endurance;
	ATTRIBUTE_ACCESSORS(UCharacterBaseAttributeSet, Endurance)
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character")
	FGameplayAttributeData Speed; // cm/s 
	ATTRIBUTE_ACCESSORS(UCharacterBaseAttributeSet, Speed)
};
