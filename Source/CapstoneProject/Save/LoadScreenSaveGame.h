// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "LoadScreenSaveGame.generated.h"

UENUM(BlueprintType)
enum ESaveSlotStatus
{
	Vacant,
	EnterName,
	Taken
};


/**
 * 
 */
UCLASS()
class CAPSTONEPROJECT_API ULoadScreenSaveGame : public USaveGame
{
	GENERATED_BODY()


public:

	UPROPERTY()
	FString SlotName = FString();

	UPROPERTY()
	int32 SlotIndex = 0;

	UPROPERTY()
	FString PlayerName = FString("Default Name");

	UPROPERTY()
	FString MapName = FString("Default Map Name");

	UPROPERTY()
	FString MapAssetName = FString("Default Map Asset Name");

	UPROPERTY()
	FName PlayerStartTag;

	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SaveSlotStatus = Vacant;

	UPROPERTY()
	bool bFirstTimeLoadIn = true;

	/* Player */

	UPROPERTY()
	int32 PlayerLevel = 1;

	UPROPERTY()
	int32 XP = 0;

	UPROPERTY()
	int32 SpellPoints = 0;

	UPROPERTY()
	int32 AttributePoints = 0;

	UPROPERTY()
	float Health = 0;

	UPROPERTY()
	float MaxHealth = 0;

	UPROPERTY()
	float Stamina = 0;

	UPROPERTY()
	float MaxStamina = 0;

	UPROPERTY()
	float Mana = 0;

	UPROPERTY()
	float MaxMana = 0;

	UPROPERTY()
	float IncomingDamage = 0;
	
};
