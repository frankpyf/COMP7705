#include "MultiSpellAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "CapstoneProject/SpellProjectile.h"
#include "CapstoneProject/Interfaces/CombatInterface.h"

UMultiSpellAbility::UMultiSpellAbility()
{
    // 初始化默认半径
    SpawnRadius = 200.0f;
}

void UMultiSpellAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UMultiSpellAbility::SpawnProjectiles(const TArray<FVector>& TargetLocations)
{
    const bool bIsAuthority = GetAvatarActorFromActorInfo()->HasAuthority();
    if (!bIsAuthority)
    {
        UE_LOG(LogTemp, Warning, TEXT("No authority to spawn projectiles."));
        return;
    }

    if (!GetAvatarActorFromActorInfo()->Implements<UCombatInterface>())
    {
        UE_LOG(LogTemp, Warning, TEXT("Avatar does not implement UCombatInterface."));
        return;
    }

    const FVector SourceLocation = ICombatInterface::Execute_GetAttackSocketLocation(GetAvatarActorFromActorInfo());
    const int32 NumProjectiles = TargetLocations.Num();

    for (int32 i = 0; i < NumProjectiles; ++i)
    {
        // 计算每个投射物的位置，均匀分布在圆环上
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
