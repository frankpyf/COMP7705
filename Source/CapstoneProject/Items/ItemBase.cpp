// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"

FPrimaryAssetId UItemBase::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(ItemType, GetFName());
}
