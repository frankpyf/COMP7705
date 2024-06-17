// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellAttackAbility.h"
#include "SpellProjectile.h"

void USpellAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	SpawnProjectile();
}

void USpellAttackAbility::SpawnProjectile()
{
	const bool bIsAuthority = GetAvatarActorFromActorInfo()->HasAuthority();
	if(!bIsAuthority)
		return;

	const auto SpawnTransform = FTransform(GetAvatarActorFromActorInfo()->GetActorRotation(),
		GetAvatarActorFromActorInfo()->GetActorLocation());
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTransform);
}
