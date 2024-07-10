#include "CapstoneAssetManager.h"
#include "CapstoneProject.h"
#include "CapstoneProject/Items/ItemBase.h"

const FPrimaryAssetType UCapstoneAssetManager::PotionType = TEXT("Potion");
const FPrimaryAssetType UCapstoneAssetManager::SkillType = TEXT("Skill");
const FPrimaryAssetType UCapstoneAssetManager::WeaponType = TEXT("Weapon");

void UCapstoneAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	// ...
}

UCapstoneAssetManager& UCapstoneAssetManager::Get()
{
	auto This = Cast<UCapstoneAssetManager>(GEngine->AssetManager);
	if(This)
		return *This;
	UE_LOG(LogCapstoneProject, Fatal, TEXT("Wrong Asset Manager"));
	return *NewObject<UCapstoneAssetManager>();
}

UItemBase* UCapstoneAssetManager::ForceLoadItem(const FPrimaryAssetId& PrimaryAssetId, bool bLogWarning)
{
	FSoftObjectPath ItemPath = GetPrimaryAssetPath(PrimaryAssetId);

	auto LoadedItemDesc = Cast<UItemBase>(ItemPath.TryLoad());
	if(bLogWarning && !LoadedItemDesc)
	{
		UE_LOG(LogCapstoneProject, Warning, TEXT("Load failed: %s"), *PrimaryAssetId.ToString());
	}
	return LoadedItemDesc;
}
