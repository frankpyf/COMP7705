#pragma once

#include "ItemBase.h"
#include "PotionItem.generated.h"

UCLASS()
class CAPSTONEPROJECT_API UPotionItem : public UItemBase
{
	GENERATED_BODY()
public:
	UPotionItem()
		: UItemBase()
	{
		ItemType = UCapstoneAssetManager::PotionType;
	}
};
