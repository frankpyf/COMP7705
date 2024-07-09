// Copyright Epic Games, Inc. All Rights Reserved.

#include "CapstoneProjectGameMode.h"
#include "CapstoneProject/CapstoneProjectPlayerController.h"
#include "GameFramework/HUD.h"
#include "UObject/ConstructorHelpers.h"
#include "CapstoneProject/Save/LoadScreenSaveGame.h"
#include "CPGameInstance.h"
#include <Kismet/GameplayStatics.h>

ACapstoneProjectGameMode::ACapstoneProjectGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ACapstoneProjectPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/RougeLike/Blueprints/Characters/Hero/BP_HeroCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/RougeLike/Blueprints/BP_BasePlayerController"));
	if(PlayerControllerBPClass.Class != nullptr)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<AHUD> HUDBPClass(TEXT("/Game/RougeLike/Blueprints/BP_HUD_Base"));
	if(HUDBPClass.Class != nullptr)
	{
		HUDClass = HUDBPClass.Class;
	}
}

ULoadScreenSaveGame* ACapstoneProjectGameMode::RetrieveInGameSaveData()
{
	UCPGameInstance* CPGameInstance = Cast<UCPGameInstance>(GetGameInstance());

	const FString InGameLoadSlotName = CPGameInstance->LoadSlotName;
	const int32 InGameLoadSlotIndex = CPGameInstance->LoadSlotIndex;

	return GetSaveSlotData(InGameLoadSlotName, InGameLoadSlotIndex);
}

ULoadScreenSaveGame* ACapstoneProjectGameMode::GetSaveSlotData(const FString& SlotName, int32 SlotIndex) const
{
	USaveGame* SaveGameObject = nullptr;
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		SaveGameObject = UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex);
	}
	else
	{
		SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	}
	ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(SaveGameObject);
	return LoadScreenSaveGame;
}

void ACapstoneProjectGameMode::GameOver()
{
	if(bGameOver == false)
	{
		OnGameOver();
		bGameOver = true;
	}
}

void ACapstoneProjectGameMode::SaveInGameProgressData(ULoadScreenSaveGame* SaveObject)
{
	UCPGameInstance* AuraGameInstance = Cast<UCPGameInstance>(GetGameInstance());

	const FString InGameLoadSlotName = AuraGameInstance->LoadSlotName;
	const int32 InGameLoadSlotIndex = AuraGameInstance->LoadSlotIndex;
	AuraGameInstance->PlayerStartTag = SaveObject->PlayerStartTag;

	UGameplayStatics::SaveGameToSlot(SaveObject, InGameLoadSlotName, InGameLoadSlotIndex);
}
