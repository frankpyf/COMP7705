// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellProjectile.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/PointLightComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ASpellProjectile::ASpellProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	RootComponent = SphereComponent;
	// SphereComponent->SetCollisionProfileName("Projectile");

	EffectComponent = CreateDefaultSubobject<UNiagaraComponent>("EffectComponent");
	EffectComponent->SetupAttachment(GetRootComponent());

	PointLight = CreateDefaultSubobject<UPointLightComponent>("PointLight");
	PointLight->SetupAttachment(GetRootComponent());

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
	MovementComp->InitialSpeed = 1000.f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;

	SphereComponent->OnComponentHit.AddUniqueDynamic(this, &ASpellProjectile::HitCallback);
}

// Called when the game starts or when spawned
void ASpellProjectile::BeginPlay()
{
	Super::BeginPlay();

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		this,
		FlashEffect,
		GetActorLocation(),
		GetActorRotation(),
		FVector(1),true, true, ENCPoolMethod::AutoRelease);
	const FLatentActionInfo LatentInfo(0, FMath::Rand(), TEXT("DestroyWrapper"), this);
	UKismetSystemLibrary::Delay(this, 10.f, LatentInfo);
}

void ASpellProjectile::HitCallback(
	UPrimitiveComponent* HitComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(1, 1, FColor::Red, TEXT("Hit"));
	const FVector Location = Hit.Location + Hit.ImpactNormal * HitOffset;
	const FRotator Rotation = UKismetMathLibrary::MakeRotFromX(Hit.ImpactNormal);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		this,
		HitEffect,
		Location,
		Rotation,
		FVector(1),true, true, ENCPoolMethod::AutoRelease);

	PointLight->SetIntensity(0.0f);
	const FLatentActionInfo LatentInfo(0, FMath::Rand(), TEXT("Destroy"), this);
	UKismetSystemLibrary::Delay(this, HitDestroyDelay, LatentInfo);
}

void ASpellProjectile::DestroyWrapper()
{
	Destroy();
}

