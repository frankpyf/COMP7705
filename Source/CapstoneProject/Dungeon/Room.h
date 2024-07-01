// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Room.generated.h"

UCLASS()
class CAPSTONEPROJECT_API ARoom : public AActor
{
	GENERATED_BODY()
	
	friend class ADungeonGenerator;
	static int32 TileUnitLen;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USplineComponent* BoundingSplineComponent;
	
	// Room has to be at least 4 tiles in X axis
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Room Size")
	int32 XLen = 10;
	// Room has to be at least 4 tiles in Y axis
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Room Size")
	int32 YLen = 10;
public:	
	// Sets default values for this actor's properties
	ARoom();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;
};
