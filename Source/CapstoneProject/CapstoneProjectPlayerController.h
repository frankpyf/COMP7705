// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "CapstoneProjectPlayerController.generated.h"

class UInputAction;
class UInputMappingContext;
class UItemBase;
/** Forward declaration to improve compiling times */
class ACapstoneProjectCharacter;

DECLARE_LOG_CATEGORY_EXTERN(Log_CSP, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSlottedItemChangedSignature, int32, SlotIdx, UItemBase*, Item);

UCLASS()
class ACapstoneProjectPlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
	TMap<UItemBase*, int32> InventoryData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
	TArray<UItemBase*> SlottedItems{nullptr, nullptr, nullptr};

	UPROPERTY(BlueprintAssignable, Category = Inventory)
	FOnSlottedItemChangedSignature SlottedItemChanged;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMapping;
public:
	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool AddItem(UItemBase* NewItem, int32 Amount);

	UFUNCTION(BlueprintCallable, Category = Slots)
	bool TryAddToSlots(UItemBase* AddingItem);

	// Try Consume Item, will return false if required amount is greater than owned
	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool ConsumeItem(UItemBase* ConsumingItem, int32 Amount);

	// Return the amount of a given item, return 0 when item is not found
	UFUNCTION(BlueprintCallable, Category = Inventory)
	int32 GetItemAmount(UItemBase* QueryItem);

	UFUNCTION(BlueprintCallable, Category = Inventory)
	UItemBase* GetItemInSlot(const int32 SlotIdx) const;

	UFUNCTION(BlueprintCallable, Category = Inventory)
	TArray<UItemBase*> GetInventoryItems() const;

	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool SaveInventory();

	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool LoadInventory();
	
protected:
	virtual void SetupInputComponent() override;
	
	// To add mapping context
	virtual void BeginPlay() override;
};


