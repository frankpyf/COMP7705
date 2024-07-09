// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemBase.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONEPROJECT_API UItemBase : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
	FPrimaryAssetType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	FText ItemDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	UTexture2D* Icon;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
