// Copyright Epic Games, Inc. All Rights Reserved.

#include "CapstoneProjectPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "Engine/LocalPlayer.h"

DEFINE_LOG_CATEGORY(Log_CSP);

void ACapstoneProjectPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	bShowMouseCursor = true;
}

bool ACapstoneProjectPlayerController::AddItem(UItemBase* NewItem, int32 Amount)
{
	if(!HasAuthority())
		return false;
	if(!NewItem)
	{
		UE_LOG(Log_CSP, Warning, TEXT("Adding Null to inventory"));
		return false;
	}
	if(Amount)
	{
		UE_LOG(Log_CSP, Warning, TEXT("Adding invalid amount"));
		return false;
	}
	int32 OwnedAmount = GetItemAmount(NewItem);
	
	if(OwnedAmount == 0)
	{
		InventoryData[NewItem] = 0;
	}
	InventoryData[NewItem] += Amount;
	return true;
}

bool ACapstoneProjectPlayerController::ConsumeItem(UItemBase* ConsumingItem, int32 Amount)
{
	if(!HasAuthority())
		return false;
	if(!ConsumingItem)
	{
		UE_LOG(Log_CSP, Warning, TEXT("Consume Null"));
		return false;
	}
	if(Amount)
	{
		UE_LOG(Log_CSP, Warning, TEXT("Consume invalid amount"));
		return false;
	}
	int32 OwnedAmount = GetItemAmount(ConsumingItem);
	if(OwnedAmount < Amount)
		return false;
	InventoryData[ConsumingItem] -= Amount;
	if(InventoryData[ConsumingItem] == 0)
	{
		InventoryData.Remove(ConsumingItem);
		for(auto& [Slot, Item] : SlottedItems)
		{
			if(Item == ConsumingItem)
			{
				Item = nullptr;
				SlottedItemChanged.Broadcast(Slot, Item);
			}
		}
	}
	return true;
}

int32 ACapstoneProjectPlayerController::GetItemAmount(UItemBase* QueryItem)
{
	const int32* FoundAmount = InventoryData.Find(QueryItem);
	return FoundAmount == nullptr ? 0 : *FoundAmount;
}

void ACapstoneProjectPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();
}
