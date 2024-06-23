// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellAttackAbility.h"
#include "SpellProjectile.h"
#include "Interfaces/CombatInterface.h"

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
	
	if(!GetAvatarActorFromActorInfo()->Implements<UCombatInterface>())
		return;
	const auto SpawnTransform = FTransform(GetAvatarActorFromActorInfo()->GetActorRotation(),
		ICombatInterface::Execute_GetAttackSocketLocation(GetAvatarActorFromActorInfo()));
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	const auto Projectile = GetWorld()->SpawnActorDeferred<ASpellProjectile>(ProjectileClass,
		SpawnTransform,
		GetAvatarActorFromActorInfo(),
		Cast<APawn>(GetAvatarActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	Projectile->FinishSpawning(SpawnTransform);
}
