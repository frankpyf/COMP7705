// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemDescriptor.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONEPROJECT_API UItemDescriptor : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	FPrimaryAssetType ItemType;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	FText ItemDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	int32 Price;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};

