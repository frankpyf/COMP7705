// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CPSaveGame.generated.h"

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
class CAPSTONEPROJECT_API UCPSaveGame : public USaveGame
{
	GENERATED_BODY()


public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SaveGame)
	TMap<FPrimaryAssetId, int32> InventoryData;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SaveGame)
	TArray<FPrimaryAssetId> SlottedItems;

	bool EncryptAndSaveToFile(const FString& SlotName, const int32 SlotIdx);
	bool LoadAndDecryptFromFile(const FString& SlotName, const int32 SlotIdx);
private:
	bool GenerateRSAKeys();
	bool LoadRSAKeys(FString& PublicKey, FString& PrivateKey);
	FString PublicKeyPath;
	FString PrivateKeyPath;
};
