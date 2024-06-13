// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DungeonStrategy.h"
#include "CapstoneProject/PortalActor.h"
#include "DungeonGenerator.generated.h"

UCLASS()
class CAPSTONEPROJECT_API ADungeonGenerator : public AActor, public ITinyKeepStrategy
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, Category="Dungeon")
	uint32 LenX = 20;
	
	UPROPERTY(EditAnywhere, Category="Dungeon")
	uint32 LenY = 20;

	UPROPERTY(EditAnywhere, Category="Dungeon")
	uint32 TileLen = 100;

	UPROPERTY(EditAnywhere, Category="Dungeon")
	uint32 RoomsToGenerate = 1;

	// Number of Tiles
	UPROPERTY(EditAnywhere, Category="Dungeon|Room")
	uint32 RoomMinLenX = 4;
	UPROPERTY(EditAnywhere, Category="Dungeon|Room")
	uint32 RoomMinLenY = 4;

	// Number of Tiles
	UPROPERTY(EditAnywhere, Category="Dungeon|Room")
	uint32 RoomMaxLenX = 8;
	UPROPERTY(EditAnywhere, Category="Dungeon|Room")
	uint32 RoomMaxLenY = 8;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ARoom> RoomActor;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACorridor> CorridorActor;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABaseInteractable> ChestActor;

	UPROPERTY(EditAnywhere)
	TSubclassOf<APortalActor> PortalActor;
	
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere)
	bool bDebug = false;
#endif
	
public:	
	// Sets default values for this actor's properties
	ADungeonGenerator();

	UFUNCTION(BlueprintCallable)
	void AddRoom(int32 X, int32 Y, int32 RoomLenX, int32 RoomLenY) const;

	UFUNCTION(BlueprintCallable)
	ACorridor* PlaceCorridor(int32 X, int32 Y) const;
	UFUNCTION(BlueprintCallable)
	void AddCorridorPoint(ACorridor* InCorridor, int32 X, int32 Y) const;
	UFUNCTION(BlueprintCallable)
	void SetSpawnPoint(const int32 X, const int32 Y);
	
	UFUNCTION(BlueprintCallable)
	void AddChest(const int32 X, const int32 Y) const;
	
	UFUNCTION(BlueprintCallable)
	void AddPortal(const int32 X, const int32 Y) const;

	UFUNCTION(BlueprintCallable)
	void GenerateDungeon();

	int GetLenX() const { return LenX; }
	int GetLenY() const { return LenY; }
	
	int GetRoomMinLenX() const { return RoomMinLenX; }
	int GetRoomMinLenY() const { return RoomMinLenY; }
	int GetRoomMaxLenX() const { return RoomMaxLenX; }
	int GetRoomMaxLenY() const { return RoomMaxLenY; }

	int GetRoomsToGenerate() const { return RoomsToGenerate; }
	
#if WITH_EDITORONLY_DATA
	bool IsInDebug() const { return bDebug; }
#endif
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void AddInternal(const int32 X, const int32 Y, const TSubclassOf<AActor>) const;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};

