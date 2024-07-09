// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "CapstoneProjectPlayerController.generated.h"

class UItemBase;
/** Forward declaration to improve compiling times */
class ACapstoneProjectCharacter;

DECLARE_LOG_CATEGORY_EXTERN(Log_CSP, Log, All);

USTRUCT(BlueprintType)
struct FItemSlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	FPrimaryAssetType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	int32 SlotIdx;
	
	bool IsValid() const
	{
		return SlotIdx >= 0 && ItemType.IsValid();
	}
};

inline bool operator==(const FItemSlot& Lhs, const FItemSlot& Rhs)
{
	return Lhs.ItemType == Rhs.ItemType && Lhs.SlotIdx == Rhs.SlotIdx;
}
inline bool operator!=(const FItemSlot& Lhs, const FItemSlot& Rhs)
{
	return !(Lhs==Rhs);
}
inline uint32 GetTypeHash(const FItemSlot& Key)
{
	uint32 Hash = 0;
	Hash = HashCombine(Hash, GetTypeHash(Key.ItemType));
	Hash = HashCombine(Hash, static_cast<uint32>(Key.SlotIdx));
	return Hash;
}
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSlottedItemChangedSignature, FItemSlot, Slot, UItemBase*, Item);

UCLASS()
class ACapstoneProjectPlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
	TMap<UItemBase*, int32> InventoryData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
	TMap<FItemSlot, UItemBase*> SlottedItems;


	UPROPERTY(BlueprintAssignable, Category = Inventory)
	FOnSlottedItemChangedSignature SlottedItemChanged;
public:
	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool AddItem(UItemBase* NewItem, int32 Amount);

	// Try Consume Item, will return false if required amount is greater than owned
	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool ConsumeItem(UItemBase* ConsumingItem, int32 Amount);

	// Return the amount of a given item, return 0 when item is not found
	UFUNCTION(BlueprintCallable, Category = Inventory)
	int32 GetItemAmount(UItemBase* QueryItem);
	
protected:
	virtual void SetupInputComponent() override;
	
	// To add mapping context
	virtual void BeginPlay() override;
};


