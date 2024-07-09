// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CapstoneProject/Interactables/BaseInteractable.h"
#include "PotionBase.generated.h"

class UItemBase;

UCLASS()
class CAPSTONEPROJECT_API APotionBase : public ABaseInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APotionBase();

	virtual void Interact_Implementation(AActor* InstigatorPawn) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* Target);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, Category="Game Effects")
	TSubclassOf<class UGameplayEffect> EffectToApply;
	
	UPROPERTY(EditDefaultsOnly)
	UItemBase* ItemType;

	UPROPERTY(EditDefaultsOnly)
	int32 AddAmount = 1;
};
