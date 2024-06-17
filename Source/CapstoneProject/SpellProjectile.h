// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpellProjectile.generated.h"

UCLASS()
class CAPSTONEPROJECT_API ASpellProjectile : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY()
	class USphereComponent* SphereComponent;

	UPROPERTY()
	class UParticleSystemComponent* EffectComponent;

	UPROPERTY(EditDefaultsOnly)
	class UProjectileMovementComponent* MovementComp;
public:	
	// Sets default values for this actor's properties
	ASpellProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
