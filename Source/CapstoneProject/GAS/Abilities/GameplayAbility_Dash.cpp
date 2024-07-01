// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbility_Dash.h"
#include "CapstoneProject/Character/BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UGameplayAbility_Dash::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}
	if (auto BaseCharacter = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo()))
	{
		if (auto MovementComp = Cast<UCharacterMovementComponent>(BaseCharacter->GetMovementComponent()))
		{
			const FVector Acceleration = MovementComp->GetCurrentAcceleration();
			const float Acceleration1D = Acceleration.Length();
			const FVector Dir = Acceleration1D > 0 ? Acceleration.GetSafeNormal2D() : BaseCharacter->GetActorForwardVector();
		}
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
