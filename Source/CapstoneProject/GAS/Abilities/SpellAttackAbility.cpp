// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellAttackAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "CapstoneProject/SpellProjectile.h"
#include "CapstoneProject/Interfaces/CombatInterface.h"

void USpellAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void USpellAttackAbility::SpawnProjectile(const FVector& TargetLocation)
{
	const bool bIsAuthority = GetAvatarActorFromActorInfo()->HasAuthority();
	if(!bIsAuthority)
		return;
	
	if(!GetAvatarActorFromActorInfo()->Implements<UCombatInterface>())
		return;
	
	const FVector SourceLocation = ICombatInterface::Execute_GetAttackSocketLocation(GetAvatarActorFromActorInfo());
	const FRotator Rotation = (TargetLocation - SourceLocation).Rotation();
	const auto SpawnTransform = FTransform(Rotation, SourceLocation);
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	auto Projectile = GetWorld()->SpawnActorDeferred<ASpellProjectile>(ProjectileClass,
		SpawnTransform,
		GetAvatarActorFromActorInfo(),
		Cast<APawn>(GetAvatarActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	if(auto SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo()))
	{
		auto SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());
		Projectile->DamageEffectSpecHandle = SpecHandle;
		Projectile->Source = GetAvatarActorFromActorInfo();
	}

	Projectile->FinishSpawning(SpawnTransform);
}
