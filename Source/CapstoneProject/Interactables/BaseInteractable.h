#pragma once

#include "CoreMinimal.h"
#include "CapstoneProject/Interfaces/InteractInterface.h"
#include "GameFramework/Actor.h"
#include "BaseInteractable.generated.h"

UCLASS()
class CAPSTONEPROJECT_API ABaseInteractable : public AActor, public IInteractInterface
{
	GENERATED_BODY()
public:
	virtual void Interact_Implementation(AActor* InstigatorPawn) override;
	virtual void Highlight_Implementation() override;
};
