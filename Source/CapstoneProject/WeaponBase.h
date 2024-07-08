// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class UCapsuleComponent;

UCLASS()
class CAPSTONEPROJECT_API AWeaponBase : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<AActor*> HitActor;
protected:
	UPROPERTY(EditAnywhere)
	UCapsuleComponent* CollisionCapsule;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* WeaponMesh;
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

	UFUNCTION(BlueprintCallable)
	void WeaponBeginAttack();
	UFUNCTION(BlueprintCallable)
	void WeaponEndAttack();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
};
