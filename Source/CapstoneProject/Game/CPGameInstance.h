// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CPGameInstance.generated.h"

class UCPSaveGame;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSaveGameLoadedNative, UCPSaveGame*)
/**
 * 
 */
UCLASS()
class CAPSTONEPROJECT_API UCPGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	FOnSaveGameLoadedNative OnSaveGameLoadedNative;
	
protected:
	UPROPERTY(BlueprintReadWrite, Category = Save)
	FString SlotName = "Test";

	UPROPERTY(BlueprintReadWrite, Category = Save)
	int32 SlotIdx = 0;
	
	UPROPERTY()
	UCPSaveGame* CurrentSaveGame;

	UPROPERTY()
	bool bSaving;

	UPROPERTY()
	bool bPendingSaveRequested;
public:
	UFUNCTION(BlueprintCallable, Category = Save)
	UCPSaveGame* GetCurrentSaveGame();

	UFUNCTION(BlueprintCallable, Category = Save)
	bool WriteSaveGame();

	UFUNCTION(BlueprintCallable, Category = Save)
	bool HandleSaveGameLoaded(class USaveGame* SaveGameObject);
protected:
	UFUNCTION(BlueprintCallable, Category = Inventory)
	void AddDefaultInventory(UCPSaveGame* SaveGame);
};
