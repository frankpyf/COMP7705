// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDescriptor.h"

FPrimaryAssetId UItemDescriptor::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(ItemType, GetFName());
}