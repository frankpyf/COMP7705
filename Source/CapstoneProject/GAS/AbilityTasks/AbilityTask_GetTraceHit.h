// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_GetTraceHit.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTraceHitTargetDataSignature, const FGameplayAbilityTargetDataHandle&, Data);
/**
 * 
 */
UCLASS()
class CAPSTONEPROJECT_API UAbilityTask_GetTraceHit : public UAbilityTask
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, meta=(HidePin="OwningAbility", DefaultToSelf="OwningAbility", BlueprintInternalUseOnly=true))
	static UAbilityTask_GetTraceHit* GetTraceHit(UGameplayAbility* OwningAbility);

private:
	virtual void Activate() override;
	void SendData() const;
	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle&, FGameplayTag);
	
	UPROPERTY(BlueprintAssignable)
	FTraceHitTargetDataSignature ValidData;
};
