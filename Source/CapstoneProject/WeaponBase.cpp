// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Character/BaseCharacter.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Default Scene Root");

	CollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>("Collision Capsule");
	CollisionCapsule->SetupAttachment(GetRootComponent());
	CollisionCapsule->AddRelativeLocation(FVector(0.f, 0.f, 20.f));
	CollisionCapsule->SetCollisionProfileName("NoCollision");
	CollisionCapsule->OnComponentBeginOverlap.AddUniqueDynamic(this, &AWeaponBase::OnHit);
	
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("Weapon Mesh");
	WeaponMesh->SetCollisionProfileName("NoCollision");
	WeaponMesh->SetupAttachment(GetRootComponent());
}

void AWeaponBase::WeaponBeginAttack()
{
	CollisionCapsule->SetCollisionProfileName("Weapon");
}

void AWeaponBase::WeaponEndAttack()
{
	CollisionCapsule->SetCollisionProfileName("NoCollision");
	HitActor.Empty();
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeaponBase::OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!OtherActor->IsA(ABaseCharacter::StaticClass()))
		return;
	if(GetInstigator()->GetClass() == OtherActor->GetClass())
		return;
	if(HitActor.Find(OtherActor) != INDEX_NONE)
		return;
	HitActor.Add(OtherActor);
	
	FGameplayEventData Payload;
	Payload.Instigator = GetInstigator();
	Payload.Target = OtherActor;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetInstigator(),
		FGameplayTag::RequestGameplayTag("Event.Attack.WeaponHit"),
		Payload);
}
