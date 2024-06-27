// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityTask_GetTraceHit.h"

#include "AbilitySystemComponent.h"
#include "CapstoneProject/Interfaces/CombatInterface.h"

UAbilityTask_GetTraceHit* UAbilityTask_GetTraceHit::GetTraceHit(UGameplayAbility* OwningAbility)
{
	UAbilityTask_GetTraceHit* MyObj = NewAbilityTask<UAbilityTask_GetTraceHit>(OwningAbility);
	return MyObj;
}

void UAbilityTask_GetTraceHit::Activate()
{
	if(Ability->GetCurrentActorInfo()->IsLocallyControlled())
	{
		// Send Data to Server
		SendData();
	}
	else
	{
		const auto SpecHandle = GetAbilitySpecHandle();
		const auto PredictionKey = GetActivationPredictionKey();

		AbilitySystemComponent->AbilityTargetDataSetDelegate(SpecHandle, PredictionKey).AddUObject(this, &UAbilityTask_GetTraceHit::OnTargetDataReplicatedCallback);
		const bool bCalledDelegate = AbilitySystemComponent->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, PredictionKey);
		if(!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}
}

void UAbilityTask_GetTraceHit::SendData() const
{
	FScopedPredictionWindow ScopedPredictionWindow(AbilitySystemComponent.Get());

	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	const auto PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult Hit;
	PC->GetHitResultUnderCursor(ECC_Visibility, false, Hit);
	Data->HitResult = Hit;

	FGameplayAbilityTargetDataHandle DataHandle;
	DataHandle.Add(Data);

	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		DataHandle,
		FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey);
	
	if(ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}

void UAbilityTask_GetTraceHit::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag)
{
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	if(ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}
