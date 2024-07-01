// Fill out your copyright notice in the Description page of Project Settings.

#include "DungeonGenerator.h"
#include "Room.h"
#include "Corridor.h"
#include "CapstoneProject/Interactables/PortalActor.h"
#include "CapstoneProject/Interactables/BaseInteractable.h"

// Sets default values
ADungeonGenerator::ADungeonGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	check(RoomMaxLenX < LenX)
	check(RoomMaxLenY < LenY)
}

void ADungeonGenerator::AddRoom(int32 X, int32 Y, int32 RoomLenX, int32 RoomLenY)
{
	if(RoomActor.Get())
	{
		// Actual World Position
		const int32 LowerLeftX = X * TileLen;
		const int32 LowerLeftY = Y * TileLen;
		FTransform RoomTransform;
		RoomTransform.SetTranslation(FVector(LowerLeftX, LowerLeftY, 0));
		auto SpawnedRoom = GetWorld()->SpawnActorDeferred<ARoom>(RoomActor, RoomTransform, this);
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

ACorridor* ADungeonGenerator::PlaceCorridor(const TArray<FVector>& InPoints)
{
	if(InPoints.Num() == 0)
		return nullptr;
	// offset on the Z so that we can draw on RVT
	const FVector Origin((InPoints[0].X + 0.5)* TileLen, (InPoints[0].Y + 0.5) * TileLen, 2.f);
	const FTransform OriginTransform(Origin);
	ACorridor* CurCorridor = GetWorld()->SpawnActorDeferred<ACorridor>(CorridorActor, OriginTransform, this);
	for(const auto& InPoint : InPoints)
	{
		CurCorridor->AddPoint(FVector((InPoint.X + 0.5)* TileLen, (InPoint.Y + 0.5) * TileLen, 2.f));
	}
	CurCorridor->FinishSpawning(OriginTransform);
	return CurCorridor;
}

void ADungeonGenerator::SetSpawnPoint(const int32 X, const int32 Y)
{
	SetActorLocation(FVector(-X * TileLen, -Y * TileLen, 0));
}

void ADungeonGenerator::AddInternal(int32 X, int32 Y, const TSubclassOf<AActor>& InActor)
{
	if(InActor)
	{
		const int32 ActualX = (X + 0.5) * TileLen;
		const int32 ActualY = (Y + 0.5) * TileLen;
		const FVector ActualWorldPos(ActualX, ActualY, 0);
		FActorSpawnParameters Params;
		Params.Owner = this;
		GetWorld()->SpawnActor<AActor>(InActor, ActualWorldPos, FRotator::ZeroRotator, Params);
	}
}

void ADungeonGenerator::Destroyed()
{
	Super::Destroyed();
	
}

void ADungeonGenerator::AddChest(int32 X, int32 Y)
{
	if(ChestActor)
	{
		AddInternal(X, Y, ChestActor);
	}
}

void ADungeonGenerator::AddPortal(int32 X, int32 Y)
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

