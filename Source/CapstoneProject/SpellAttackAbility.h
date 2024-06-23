// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "SpellAttackAbility.generated.h"

class ASpellProjectile;
/**
 * 
 */
UCLASS()
class CAPSTONEPROJECT_API USpellAttackAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UFUNCTION(BlueprintCallable)
	void SpawnProjectile();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASpellProjectile> ProjectileClass;
};
