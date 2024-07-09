// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONEPROJECT_API AEnemyCharacter : public ABaseCharacter
{
	
	GENERATED_BODY()
public:
	AEnemyCharacter();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void StartMeleeTrace();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void EndMeleeTrace();
	
protected:
	virtual void BeginPlay() override;
};
