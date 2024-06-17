#include "CapstoneAssetManager.h"
#include "CapstoneProject.h"
#include "Descriptors/ItemDescriptor.h"

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

UItemDescriptor* UCapstoneAssetManager::ForceLoadItem(const FPrimaryAssetId& PrimaryAssetId, bool bLogWarning)
{
	FSoftObjectPath ItemPath = GetPrimaryAssetPath(PrimaryAssetId);

	auto LoadedItemDesc = Cast<UItemDescriptor>(ItemPath.TryLoad());
	if(bLogWarning && !LoadedItemDesc)
	{
		UE_LOG(LogCapstoneProject, Warning, TEXT("Load failed: %s"), *PrimaryAssetId.ToString());
	}
	return LoadedItemDesc;
}
