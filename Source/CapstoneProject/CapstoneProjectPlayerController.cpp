// Copyright Epic Games, Inc. All Rights Reserved.

#include "CapstoneProjectPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "Engine/LocalPlayer.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

void ACapstoneProjectPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

}

void ACapstoneProjectPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

}