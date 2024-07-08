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

public:
	ACapstoneProjectGameMode();

	ULoadScreenSaveGame* RetrieveInGameSaveData();
	ULoadScreenSaveGame* GetSaveSlotData(const FString& SlotName, int32 SlotIndex) const;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USaveGame> LoadScreenSaveGameClass;

	void SaveInGameProgressData(ULoadScreenSaveGame* SaveObject);
};



