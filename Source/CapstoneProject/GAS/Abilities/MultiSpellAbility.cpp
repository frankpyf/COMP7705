#include "MultiSpellAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "CapstoneProject/SpellProjectile.h"
#include "CapstoneProject/Interfaces/CombatInterface.h"


void UMultiSpellAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UMultiSpellAbility::SpawnProjectiles(const TArray<FVector>& TargetLocations)
{
    // Server only
    const bool bIsAuthority = GetAvatarActorFromActorInfo()->HasAuthority();
    if (!bIsAuthority)
        return;

    if (!GetAvatarActorFromActorInfo()->Implements<UCombatInterface>())
        return;

    const FVector SourceLocation = ICombatInterface::Execute_GetAttackSocketLocation(GetAvatarActorFromActorInfo());
    const int32 NumProjectiles = TargetLocations.Num();

    for (int32 i = 0; i < NumProjectiles; ++i)
    {
        // ����ÿ��Ͷ�����λ�ã����ȷֲ���Բ����
        const float Angle = (2.0f * PI / NumProjectiles) * i;
        const FVector Offset = FVector(FMath::Cos(Angle) * SpawnRadius, FMath::Sin(Angle) * SpawnRadius, 0.0f);
        const FVector SpawnLocation = SourceLocation + Offset;
        const FRotator Rotation = (TargetLocations[i] - SpawnLocation).Rotation();
        const FTransform SpawnTransform(Rotation, SpawnLocation);
        FActorSpawnParameters Params;
        Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        if (!ProjectileClass)
        {
            UE_LOG(LogTemp, Error, TEXT("ProjectileClass is not set."));
            return;
        }

        auto Projectile = GetWorld()->SpawnActorDeferred<ASpellProjectile>(
            ProjectileClass,
            SpawnTransform,
            GetAvatarActorFromActorInfo(),
            Cast<APawn>(GetAvatarActorFromActorInfo()),
            ESpawnActorCollisionHandlingMethod::AlwaysSpawn
        );

        if (!Projectile)
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to spawn projectile."));
            continue;
        }

        if (auto SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo()))
        {
            auto SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());
            Projectile->DamageEffectSpecHandle = SpecHandle;
            Projectile->Source = GetAvatarActorFromActorInfo();
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to get AbilitySystemComponent."));
        }

        Projectile->FinishSpawning(SpawnTransform);
        UE_LOG(LogTemp, Log, TEXT("Projectile spawned successfully at location: %s, targeting: %s"), *SpawnLocation.ToString(), *TargetLocations[i].ToString());
    }
}
