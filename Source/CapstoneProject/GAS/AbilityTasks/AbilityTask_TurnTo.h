// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_TurnTo.generated.h"

class UTimelineComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTurnedSignature);

/**
 * 
 */
UCLASS()
class CAPSTONEPROJECT_API UAbilityTask_TurnTo : public UAbilityTask
{
	GENERATED_BODY()

	// Input
	UPROPERTY(Replicated)
	FVector Location;

	FRotator DeltaRotation;

	UPROPERTY(Replicated)
	float DurationOfTurning;

	float ElapsedTime = 0.0f;

	bool bFinished = false;

	//Output
	UPROPERTY(BlueprintAssignable)
	FTurnedSignature Completed;
	
	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;
public:
	UFUNCTION(BlueprintCallable, meta=(HidePin="OwningAbility", DefaultToSelf="OwningAbility", BlueprintInternalUseOnly=true))
	static UAbilityTask_TurnTo* TurnTo(UGameplayAbility* OwningAbility, const FVector& TargetLocation, float Duration);
};
