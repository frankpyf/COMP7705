// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "CapstoneProject.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/PointLightComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASpellProjectile::ASpellProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	RootComponent = SphereComponent;
	SphereComponent->SetCollisionObjectType(ECC_Projectile);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	EffectComponent = CreateDefaultSubobject<UNiagaraComponent>("EffectComponent");
	EffectComponent->SetupAttachment(GetRootComponent());

	PointLight = CreateDefaultSubobject<UPointLightComponent>("PointLight");
	PointLight->SetupAttachment(GetRootComponent());
	PointLight->AttenuationRadius = 200;

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
	MovementComp->InitialSpeed = 1500.f;
	MovementComp->ProjectileGravityScale = 0.001f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;

	SphereComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &ASpellProjectile::HitCallback);
}

// Called when the game starts or when spawned
void ASpellProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(10.f);
	SetReplicateMovement(true);

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		this,
		FlashEffect,
		GetActorLocation(),
		GetActorRotation(),
		FVector(1),true, true, ENCPoolMethod::AutoRelease);
}

void ASpellProjectile::HitCallback(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult)
{
	if(OtherActor == Source)
		return;
	const FVector Location = SweepResult.Location + SweepResult.ImpactNormal * HitOffset;
	const FRotator Rotation = UKismetMathLibrary::MakeRotFromX(SweepResult.ImpactNormal);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		this,
		HitEffect,
		Location,
		Rotation,
		FVector(1),true, true, ENCPoolMethod::AutoRelease);

	if(HasAuthority())
	{
		if(auto TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
		}
		PointLight->SetIntensity(0.0f);
		Destroy();
	}
}

