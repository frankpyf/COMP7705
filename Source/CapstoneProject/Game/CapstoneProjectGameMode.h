// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CapstoneProjectGameMode.generated.h"

class ULoadScreenSaveGame;
class USaveGame;

UCLASS(minimalapi)
class ACapstoneProjectGameMode : public AGameModeBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USaveGame> LoadScreenSaveGameClass;
public:
	ACapstoneProjectGameMode();

	ULoadScreenSaveGame* RetrieveInGameSaveData();
	ULoadScreenSaveGame* GetSaveSlotData(const FString& SlotName, int32 SlotIndex) const;

	UFUNCTION(BlueprintCallable)
	virtual void GameOver();

	void SaveInGameProgressData(ULoadScreenSaveGame* SaveObject);
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnGameOver();

	UPROPERTY(BlueprintReadWrite)
	int32 GameLevel = 0;

	UPROPERTY(BlueprintReadOnly)
	uint8 bGameOver : 1;
};



