// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "SpellProjectile.generated.h"

class UPointLightComponent;

UCLASS()
class CAPSTONEPROJECT_API ASpellProjectile : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	class USphereComponent* SphereComponent;
public:
	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn=true))
	FGameplayEffectSpecHandle DamageEffectSpecHandle;

	UPROPERTY()
	AActor* Source;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Niagara")
	class UNiagaraComponent* EffectComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Niagara")
	class UNiagaraSystem* FlashEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Niagara")
	UNiagaraSystem* HitEffect;

	UPROPERTY(EditDefaultsOnly)
	UPointLightComponent* PointLight;

	UPROPERTY(EditDefaultsOnly)
	class UProjectileMovementComponent* MovementComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Delay")
	float HitDestroyDelay = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float HitOffset;
public:	
	// Sets default values for this actor's properties
	ASpellProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void HitCallback(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
};
