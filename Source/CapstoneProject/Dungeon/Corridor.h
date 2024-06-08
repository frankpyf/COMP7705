// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Corridor.generated.h"

UCLASS()
class CAPSTONEPROJECT_API ACorridor : public AActor
{
	GENERATED_BODY()
	friend class ADungeonGenerator;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USplineComponent* CorridorSplineComponent;

public:	
	// Sets default values for this actor's properties
	ACorridor();

	UFUNCTION(BlueprintCallable)
	void AddPoint(const FVector& InPos);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
