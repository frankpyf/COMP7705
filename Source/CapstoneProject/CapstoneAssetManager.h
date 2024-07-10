#pragma once

#include "Engine/AssetManager.h"
#include "CapstoneAssetManager.generated.h"

UCLASS()
class UCapstoneAssetManager : public UAssetManager
{
	GENERATED_BODY()
public:
	static const FPrimaryAssetType PotionType;
	static const FPrimaryAssetType SkillType;
	static const FPrimaryAssetType WeaponType;
	
	virtual void StartInitialLoading() override;

	static UCapstoneAssetManager& Get();
	
	class UItemBase* ForceLoadItem(const FPrimaryAssetId& PrimaryAssetId, bool bLogWarning = true);
};
