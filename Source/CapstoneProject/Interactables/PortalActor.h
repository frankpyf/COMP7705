// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseInteractable.h"
#include "PortalActor.generated.h"

UCLASS()
class CAPSTONEPROJECT_API APortalActor : public ABaseInteractable
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mesh", meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Levels", meta=(AllowPrivateAccess="true"))
	TArray<FName> LevelsToLoad;

	static bool bIsPreparing;
public:	
	// Sets default values for this actor's properties
	APortalActor();

	UFUNCTION(BlueprintCallable)
	bool GetIsPreparing() const { return bIsPreparing; }

	UFUNCTION(BlueprintCallable)
	void FlipIsPreparing() { bIsPreparing = !bIsPreparing; }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
