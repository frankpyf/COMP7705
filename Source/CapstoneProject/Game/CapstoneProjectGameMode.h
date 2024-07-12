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

	UFUNCTION(BlueprintCallable)
	virtual void GameOver();
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnGameOver();

	UPROPERTY(BlueprintReadWrite)
	int32 GameLevel = 0;

	UPROPERTY(BlueprintReadOnly)
	uint8 bGameOver : 1;
};



