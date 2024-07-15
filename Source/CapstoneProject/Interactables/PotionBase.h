// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseInteractable.h"
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

public:
	UPROPERTY(EditAnywhere, Category="Game Effects")
	TSubclassOf<class UGameplayAbility> AbilityToGive;
protected:
	UPROPERTY(EditDefaultsOnly)
	UItemBase* ItemType;

	UPROPERTY(EditDefaultsOnly)
	int32 AddAmount = 1;
};
