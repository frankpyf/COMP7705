// Copyright Epic Games, Inc. All Rights Reserved.

#include "CapstoneProjectPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "Engine/LocalPlayer.h"
#include "Game/CPGameInstance.h"
#include "Items/ItemBase.h"
#include "Save/CPSaveGame.h"

DEFINE_LOG_CATEGORY(Log_CSP);

bool ACapstoneProjectPlayerController::AddItem(UItemBase* NewItem, int32 Amount)
{
	if(!NewItem)
	{
		UE_LOG(Log_CSP, Warning, TEXT("Adding Null to inventory"));
		return false;
	}
	if(Amount < 1)
	{
		UE_LOG(Log_CSP, Warning, TEXT("Adding invalid amount"));
		return false;
	}
	int32 OwnedAmount = InventoryData.FindOrAdd(NewItem);
	if(OwnedAmount == 0)
	{
		TryAddToSlots(NewItem);
	}
	InventoryData[NewItem] = OwnedAmount + Amount;
	// SaveInventory();
	return true;
}

bool ACapstoneProjectPlayerController::TryAddToSlots(UItemBase* AddingItem)
{
	for(int32 Idx= 0; Idx < 3; ++Idx)
	{
		if(SlottedItems[Idx] == nullptr)
		{
			SlottedItems[Idx] = AddingItem;
			SlottedItemChanged.Broadcast(Idx, AddingItem);
			return true;
		}
	}
	return false;
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
		for(int32 SlotIdx = 0; SlotIdx < SlottedItems.Num(); ++SlotIdx)
		{
			if(SlottedItems[SlotIdx] == ConsumingItem)
			{
				SlottedItems[SlotIdx] = nullptr;
				SlottedItemChanged.Broadcast(SlotIdx, SlottedItems[SlotIdx]);
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

UItemBase* ACapstoneProjectPlayerController::GetItemInSlot(const int32 SlotIdx) const
{
	if(!SlottedItems.IsValidIndex(SlotIdx))
		return nullptr;
	return SlottedItems[SlotIdx];
}

TArray<UItemBase*> ACapstoneProjectPlayerController::GetInventoryItems() const
{
	TArray<UItemBase*> OwnedItems;
	InventoryData.GetKeys(OwnedItems);
	GEngine->AddOnScreenDebugMessage(2, 2, FColor::Red, FString::Printf(TEXT("%d"), OwnedItems.Num()));
	return OwnedItems;
}

bool ACapstoneProjectPlayerController::SaveInventory()
{
	const auto World = GetWorld();
	const auto GameInstance = World? World->GetGameInstance<UCPGameInstance>() : nullptr;
	if(!GameInstance)
		return false;

	auto CurrentSaveGame = GameInstance->GetCurrentSaveGame();
	if(CurrentSaveGame)
	{
		CurrentSaveGame->InventoryData.Reset();
		CurrentSaveGame->SlottedItems.Reset();

		for(const auto [Key, Value] : InventoryData)
		{
			if(Key)
			{
				FPrimaryAssetId AssetId = Key->GetPrimaryAssetId();
				CurrentSaveGame->InventoryData.Add(AssetId, Value);
			}
		}
		for(const auto Item : SlottedItems)
		{
			FPrimaryAssetId AssetId;
			if(Item)
			{
				AssetId = Item->GetPrimaryAssetId();
			}
			CurrentSaveGame->SlottedItems.Add(AssetId);
		}
		GameInstance->WriteSaveGame();
		return true;
	}
	return false;
}

bool ACapstoneProjectPlayerController::LoadInventory()
{
	InventoryData.Reset();
	const auto World = GetWorld();
	const auto GameInstance = World? World->GetGameInstance<UCPGameInstance>() : nullptr;

	if(!GameInstance)
		return false;
	
	auto CurrentSaveGame = GameInstance->GetCurrentSaveGame();
	if(CurrentSaveGame)
	{
		UCapstoneAssetManager& AssetManager = UCapstoneAssetManager::Get();
		for(const auto [Key, Value] : CurrentSaveGame->InventoryData)
		{
			auto* LoadedItem = AssetManager.ForceLoadItem(Key);
			if(LoadedItem)
			{
				InventoryData.Add(LoadedItem, Value);
			}
		}
		for(int32 Idx = 0; Idx < CurrentSaveGame->SlottedItems.Num(); ++Idx)
		{
			auto* LoadedItem = AssetManager.ForceLoadItem(CurrentSaveGame->SlottedItems[Idx]);
			if(LoadedItem)
			{
				SlottedItems[Idx] = LoadedItem;
				SlottedItemChanged.Broadcast(Idx, LoadedItem);
			}
		}
		return true;
	}
	return false;
}

void ACapstoneProjectPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();
}

void ACapstoneProjectPlayerController::BeginPlay()
{
	const auto World = GetWorld();
	const auto GameInstance = World? World->GetGameInstance<UCPGameInstance>() : nullptr;
	GameInstance->OnSaveGameLoadedNative.AddLambda([this](UCPSaveGame* NewSaveGame)
	{
		LoadInventory();
	});
	
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if(auto Subsystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(InputMapping, 1);
	}
	bShowMouseCursor = true;
}