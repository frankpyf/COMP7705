// Fill out your copyright notice in the Description page of Project Settings.


#include "CPGameInstance.h"
#include "CapstoneProject/Save/CPSaveGame.h"
#include "Kismet/GameplayStatics.h"

UCPSaveGame* UCPGameInstance::GetCurrentSaveGame()
{
	return CurrentSaveGame;
}

bool UCPGameInstance::WriteSaveGame()
{
	if(bSaving)
	{
		bPendingSaveRequested = true;
		return true;
	}
	
	bSaving = true;
	//if (CurrentSaveGame && CurrentSaveGame->EncryptAndSaveToFile(SlotName, SlotIdx))
	UGameplayStatics::AsyncSaveGameToSlot(GetCurrentSaveGame(), SlotName, SlotIdx, FAsyncSaveGameToSlotDelegate::CreateLambda(
		[this](const FString& Name, const int32 Idx, bool bSuccess)
		{
			ensure(bSaving);
			bSaving = false;

			if(bPendingSaveRequested)
			{
				bPendingSaveRequested = false;
				WriteSaveGame();
			}
			GEngine->AddOnScreenDebugMessage(1, 5, FColor::Red, TEXT("Saved"));
		}));
	return true;
}

bool UCPGameInstance::HandleSaveGameLoaded(USaveGame* SaveGameObject)
{
	bool bLoaded = false;

	CurrentSaveGame = Cast<UCPSaveGame>(SaveGameObject);
	if(CurrentSaveGame)
	{
		bLoaded = true;
	}
	else
	{
		//CurrentSaveGame->LoadAndDecryptFromFile(SlotName, SlotIdx);
		CurrentSaveGame = Cast<UCPSaveGame>(UGameplayStatics::CreateSaveGameObject(UCPSaveGame::StaticClass()));

		// ..
	}
	OnSaveGameLoadedNative.Broadcast(CurrentSaveGame);
	return bLoaded;
}

void UCPGameInstance::AddDefaultInventory(UCPSaveGame* SaveGame)
{
	// for(const auto& [Key, Value] : Default)
}
