// Copyright Epic Games, Inc. All Rights Reserved.

#include "CapstoneProjectGameMode.h"
#include "CapstoneProject/CapstoneProjectPlayerController.h"
#include "GameFramework/HUD.h"
#include "UObject/ConstructorHelpers.h"

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