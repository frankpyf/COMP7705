// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CAPSTONEPROJECT_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()
	
	UPROPERTY()
	AActor* LastActor = nullptr;

	UPROPERTY()
	AActor* FocusedActor = nullptr;

	UPROPERTY()
	FVector MouseWorldLocation;
	
public:	
	// Sets default values for this component's properties
	UInteractionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	// Begin of Interaction Component Interfaces

	UFUNCTION(BlueprintCallable)
	FVector GetMouseWorldLocation() const { return MouseWorldLocation; }

	UFUNCTION(BlueprintCallable)
	void DoInteract();
protected:
	// Or Unreliable
	UFUNCTION(Server, Reliable)
	void ServerInteract(AActor* InFocusedActor);

	void FindBestInteract();
	// End of Interaction Component Interfaces
};
