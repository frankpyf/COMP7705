// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "MultiSpellAbility.generated.h"

class ASpellProjectile;

/**
 *
 */
UCLASS()
class CAPSTONEPROJECT_API UMultiSpellAbility : public UGameplayAbility
{
    GENERATED_BODY()

public:
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
    UFUNCTION(BlueprintCallable)
    void SpawnProjectiles(const TArray<FVector>& TargetLocations);

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<ASpellProjectile> ProjectileClass;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<class UGameplayEffect> DamageEffectClass;

    // ÃÌº”SpawnRadius Ù–‘
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
    float SpawnRadius;

public:
    UMultiSpellAbility();
};
