// Fill out your copyright notice in the Description page of Project Settings.

#include "DungeonGenerator.h"
#include "Room.h"
#include "Corridor.h"
#include "CapstoneProject/PortalActor.h"
#include "CapstoneProject/Interactables/BaseInteractable.h"
#include "Components/SplineComponent.h"

// Sets default values
ADungeonGenerator::ADungeonGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	check(RoomMaxLenX < LenX)
	check(RoomMaxLenY < LenY)
}

void ADungeonGenerator::AddRoom(int32 X, int32 Y, int32 RoomLenX, int32 RoomLenY) const
{
	if(RoomActor.Get())
	{
		// Actual World Position
		const int32 LowerLeftX = X * TileLen;
		const int32 LowerLeftY = Y * TileLen;
		FTransform RoomTransform;
		RoomTransform.SetTranslation(FVector(LowerLeftX, LowerLeftY, 0));
		auto SpawnedRoom = GetWorld()->SpawnActorDeferred<ARoom>(RoomActor, RoomTransform);
		SpawnedRoom->XLen = RoomLenX;
		SpawnedRoom->YLen = RoomLenY;
		ARoom::TileUnitLen = TileLen;
		SpawnedRoom->FinishSpawning(RoomTransform);
		
#if WITH_EDITOR
		if(bDebug)
		{
			DrawDebugBox(GetWorld(),
				FVector(LowerLeftX + RoomLenX * TileLen / 2, LowerLeftY + RoomLenY * TileLen / 2, 0),
				FVector(RoomLenX * TileLen / 2, RoomLenY * TileLen / 2, TileLen),
				FColor::Green, true);
		}
#endif
	}
}

ACorridor* ADungeonGenerator::PlaceCorridor(int32 X, int32 Y) const
{
	if(CorridorActor.Get())
	{
		const FVector ActualWorldPos = FVector((X + 0.5) * TileLen, (Y + 0.5) * TileLen, 0);
		auto CurCorridor = GetWorld()->SpawnActor<ACorridor>(CorridorActor, ActualWorldPos, FRotator::ZeroRotator);
		return CurCorridor;
	}
	return nullptr;
}

void ADungeonGenerator::AddCorridorPoint(ACorridor* InCorridor, int32 X, int32 Y) const
{
	if(InCorridor)
	{
		const FVector ActualWorldPos = FVector((X + 0.5) * TileLen, (Y + 0.5) * TileLen, 0);
		InCorridor->AddPoint(ActualWorldPos);
	}
}

void ADungeonGenerator::SetSpawnPoint(const int32 X, const int32 Y)
{
	SetActorLocation(FVector(-X * TileLen, -Y * TileLen, 0));
}

void ADungeonGenerator::AddInternal(int32 X, int32 Y, const TSubclassOf<AActor> InActor) const
{
	if(InActor)
	{
		const int32 ActualX = (X + 0.5) * TileLen;
		const int32 ActualY = (Y + 0.5) * TileLen;
		const FVector ActualWorldPos(ActualX, ActualY, 0);
		GetWorld()->SpawnActor(InActor, &ActualWorldPos);
	}
}

void ADungeonGenerator::AddChest(int32 X, int32 Y) const
{
	if(ChestActor)
	{
		AddInternal(X, Y, ChestActor);
	}
}

void ADungeonGenerator::AddPortal(int32 X, int32 Y) const
{
	if(PortalActor)
	{
		AddInternal(X, Y, PortalActor);
	}
}

// Called when the game starts or when spawned
void ADungeonGenerator::BeginPlay()
{
	Super::BeginPlay();
	GenerateDungeon();
}

void ADungeonGenerator::GenerateDungeon()
{
	auto Data = DoGenerate(*this);
#if WITH_EDITOR
	if(bDebug)
	{
		for(int32 Row = LenX - 1; Row >= 0; --Row)
		{
			FString RowString;
			for(uint32 Col = 0; Col < LenY; ++Col)
			{
				FColor DebugColor = FColor::Blue;
				if(Data[Row * LenY + Col] == EDungeonType::Corridor)
					DebugColor = FColor::Green;
				DrawDebugBox(GetWorld(),
					FVector((Row + 0.5)* TileLen, (Col + 0.5) * TileLen, 0),
					FVector(TileLen * 0.5, TileLen * 0.5, 10),
					DebugColor, true);
				RowString.AppendChar(StaticCast<char>(Data[Row * LenY + Col]));
				RowString.AppendChar(' ');
			}
			UE_LOG(LogTemp, Log, TEXT("%s\n"), *RowString);
		}
	}
#endif
}

// Called every frame
void ADungeonGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

