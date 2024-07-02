#include "DungeonStrategy.h"
#include "DungeonGenerator.h"
#include "Corridor.h"
#include "CompGeom/PolygonTriangulation.h"

namespace DungeonGenerationUtils
{
	struct FNode
	{
		FIntPoint Pos;
		int32 Cost = TNumericLimits<int32>::Max();
		int32 ParentIdx = -1;
		
		explicit FNode() = default;
		explicit FNode(const FIntPoint& InPos)
			:Pos(InPos) {}
	};

	FORCEINLINE uint32 GetTypeHash(const FNode& InNode)
	{
		return GetTypeHash(InNode.Pos);
	}
	
	bool operator==(const FNode& Lhs, const FNode& Rhs)
	{
		return Lhs.Pos == Rhs.Pos;
	}
	
	bool operator==(const FNode& Lhs, const FIntPoint& Rhs)
	{
		return Lhs.Pos == Rhs;
	}
	
	TArray<FIntPoint> AStar(const ADungeonGenerator& InGenerator, const TArray<EDungeonType>& Data, const FBox2d& Src, const FBox2d& Dst)
	{
		auto Comp = [&](const FNode& Lhs, const FNode& Rhs)
		{
			return Lhs.Cost < Rhs.Cost;
		};

		// Begin of A* Algorithm
		TArray<FIntPoint> ShortestPath;
		TArray<FNode> OpenSet;
		TArray<FNode> CloseSet;

		FNode SrcNode(Src.GetCenter().IntPoint());
		SrcNode.Cost = 0;
		OpenSet.Push(SrcNode);
		
		FIntPoint Directions[4] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
		
		while(!OpenSet.IsEmpty())
		{
			OpenSet.Heapify(Comp);
			FNode CurNode;
			OpenSet.HeapPop(CurNode, Comp);
			if(CurNode == Dst.GetCenter().IntPoint())
			{
				ShortestPath.Add(CurNode.Pos);
				int32 ParentNodeIdx = CurNode.ParentIdx;
				while(ParentNodeIdx != -1)
				{
					ShortestPath.Add(CloseSet[ParentNodeIdx].Pos);
					ParentNodeIdx = CloseSet[ParentNodeIdx].ParentIdx;
				}
				return ShortestPath;
			}

			for(const auto Direction : Directions)
			{
				const auto NeighborPos(Direction + CurNode.Pos);

				if(NeighborPos.X < 0 || NeighborPos.X >= InGenerator.GetLenX() ||
					NeighborPos.Y < 0 || NeighborPos.Y >= InGenerator.GetLenY() ||
					CloseSet.ContainsByPredicate([&](const FNode& ClosedNode)
					{
						return ClosedNode.Pos == NeighborPos;
					}))
					continue;
				auto FoundNode = OpenSet.FindByPredicate([&](const FNode& CandidateNode)
					{
						return CandidateNode.Pos == NeighborPos;
					});
				double CurCost = 1;
				if(Data[NeighborPos.X * InGenerator.GetLenX() + NeighborPos.Y] == EDungeonType::Corridor)
					CurCost -= 1;
				else if(Data[NeighborPos.X * InGenerator.GetLenX() + NeighborPos.Y] == EDungeonType::Wall)
					CurCost += 1;
				
				const double Cost = CurNode.Cost + CurCost + FVector2d::Distance(FVector2d(NeighborPos.X, NeighborPos.Y), Dst.GetCenter());
				if(FoundNode == nullptr) // Insert
				{
					FoundNode = &OpenSet.Emplace_GetRef(NeighborPos);
				}
				if(Cost < FoundNode->Cost)
				{
					FoundNode->Cost = Cost;
					FoundNode->ParentIdx = CloseSet.Num();
				}
			}
			CloseSet.Add(MoveTemp(CurNode));
		}
		return ShortestPath;
	}
		
	TArray<FBox2d> RandomlyPlaceRooms(ADungeonGenerator& InGenerator, TArray<EDungeonType>& InDungeon)
	{
		const auto CheckTilesAvailable = [&](const FIntPoint LowerLeft, const FIntPoint Extent)
		{
			if(LowerLeft.X + Extent.X < 0 || LowerLeft.X + Extent.X >= InGenerator.GetLenX() ||
				LowerLeft.Y + Extent.Y < 0 || LowerLeft.Y + Extent.Y >= InGenerator.GetLenY())
				return false;
			for(int32 I = LowerLeft.X; I < LowerLeft.X + Extent.X; ++I)
			{
				for(int32 J = LowerLeft.Y; J < LowerLeft.Y + Extent.Y; ++J)
				{
					if(InDungeon[I * InGenerator.GetLenY() + J] != EDungeonType::None)
						return false;
				}
			}
			return true;
		};
		
		TArray<FBox2d> RoomCenter;
		uint32 RoomsGenerated = 0;
		// Randomly place rooms as much as possible
		for(auto Attempt = 0; Attempt < 1000; ++Attempt)
		{
			const uint32 RandX = FMath::RandRange(1, InGenerator.GetLenX() - 1);
			const uint32 RandY = FMath::RandRange(1, InGenerator.GetLenY() - 1);
			const uint32 RandLenX = FMath::RandRange(InGenerator.GetRoomMinLenX(), InGenerator.GetRoomMaxLenX());
			const uint32 RandLenY = FMath::RandRange(InGenerator.GetRoomMinLenX(), InGenerator.GetRoomMaxLenY());
			
			if(CheckTilesAvailable(FIntPoint(RandX - 1, RandY - 1), FIntPoint(RandLenX + 2, RandLenY + 2)))
			{
				InGenerator.AddRoom(RandX, RandY, RandLenX, RandLenY);
				// Place Walls Surrounding a room
				// Trying to find a fill function
				for(uint32 j = RandY; j < RandY + RandLenY; ++j)
				{
					InDungeon[RandX * InGenerator.GetLenY() + j] = EDungeonType::Wall;
					InDungeon[(RandX + RandLenX - 1) * InGenerator.GetLenY() + j] = EDungeonType::Wall;
				}
				for(uint32 i = RandX + 1; i < RandX + RandLenX - 1; ++i)
				{
					InDungeon[i * InGenerator.GetLenY() + RandY] = EDungeonType::Wall;
					for(uint32 j = RandY + 1; j < RandY + RandLenY - 1; ++j)
					{
						InDungeon[i * InGenerator.GetLenY() + j] = EDungeonType::Floor;
					}
					InDungeon[i * InGenerator.GetLenY() + RandY + RandLenY - 1] = EDungeonType::Wall;
					
				}
				FIntPoint LowerLeft(RandX, RandY);
				FIntPoint UpperRight(RandX + RandLenX, RandY + RandLenY);
				RoomCenter.Emplace(MoveTemp(LowerLeft), MoveTemp(UpperRight));
				
				if(++RoomsGenerated == InGenerator.GetRoomsToGenerate())
					break;
			}
		}
		return RoomCenter;
	}
}

TArray<EDungeonType> ILinearStrategy::DoGenerate(ADungeonGenerator& InGenerator)
{
	TArray<EDungeonType> Data;
	Data.Init(EDungeonType::None, InGenerator.GetLenX() * InGenerator.GetLenY());
	// place rooms
	auto Rooms = DungeonGenerationUtils::RandomlyPlaceRooms(InGenerator, Data);
	if (Rooms.IsEmpty())
	{
		UE_LOG(LogActor, Warning, TEXT("Dungeon Generate 0 rooms with Dungeon size: %d, %d"), InGenerator.GetLenX(), InGenerator.GetLenY());
		return Data;
	}
	
	Rooms.Sort([](const auto& Room1, const auto& Room2)
	{
		return Room1.GetCenter().ComponentwiseAllLessThan(Room2.GetCenter());
	});
	
	for (int Idx = 0; Idx < Rooms.Num() - 1; ++Idx)
	{
		const auto ShortestPath = DungeonGenerationUtils::AStar(InGenerator, Data, Rooms[Idx], Rooms[Idx+1]);
		
		TArray<FVector> CorridorPoints;
		for(const auto CurPoint : ShortestPath)
		{
			auto& CurTile = Data[CurPoint.X * InGenerator.GetLenY() + CurPoint.Y];
			if(Rooms[Idx+1].IsInside(CurPoint))
				continue;
			if(Rooms[Idx].IsInside(CurPoint))
				break;
			CorridorPoints.Add(FVector(CurPoint.X, CurPoint.Y, 0.f));
			CurTile = EDungeonType::Corridor;
		}
		InGenerator.PlaceCorridor(CorridorPoints);
	}
	return Data;
}

TArray<EDungeonType> ITinyKeepStrategy::DoGenerate(ADungeonGenerator& InGenerator)
{
	TArray<EDungeonType> Data;
	Data.Init(EDungeonType::None, InGenerator.GetLenX() * InGenerator.GetLenY());
	
	auto Rooms = DungeonGenerationUtils::RandomlyPlaceRooms(InGenerator, Data);
	if (Rooms.IsEmpty())
	{
		UE_LOG(LogActor, Warning, TEXT("Dungeon Generate 0 rooms with Dungeon size: %d, %d"), InGenerator.GetLenX(), InGenerator.GetLenY());
		return Data;
	}
	
	TArray<UE::Geometry::FIndex3i> Triangles;
	TArray<FVector2d> RoomCenter;
	for(const auto CurRoom : Rooms)
	{
		RoomCenter.Emplace(CurRoom.GetCenter());
	}
	PolygonTriangulation::TriangulateSimplePolygon(RoomCenter, Triangles);
	
	// Initialize Adjacency List
	TArray<double> AdjacencyList;
	AdjacencyList.Init(TNumericLimits<double>::Max(), Rooms.Num() * Rooms.Num());
	
	for(const auto Indices : Triangles)
	{
		const auto IdxA = Indices.A;
		const auto IdxB = Indices.B;
		const auto IdxC = Indices.C;
		const auto DistAB = UE::Geometry::Distance(RoomCenter[IdxA], RoomCenter[IdxB]);
		const auto DistAC = UE::Geometry::Distance(RoomCenter[IdxA], RoomCenter[IdxC]);
		const auto DistBC = UE::Geometry::Distance(RoomCenter[IdxB], RoomCenter[IdxC]);
		AdjacencyList[IdxA * RoomCenter.Num() + IdxB] = FMath::Min(AdjacencyList[IdxA * RoomCenter.Num() + IdxB], DistAB);
		AdjacencyList[IdxA * RoomCenter.Num() + IdxC] = FMath::Min(AdjacencyList[IdxA * RoomCenter.Num() + IdxC], DistAC);
		AdjacencyList[IdxB * RoomCenter.Num() + IdxA] = FMath::Min(AdjacencyList[IdxB * RoomCenter.Num() + IdxA], DistAB);
		AdjacencyList[IdxB * RoomCenter.Num() + IdxC] = FMath::Min(AdjacencyList[IdxB * RoomCenter.Num() + IdxC], DistBC);
		AdjacencyList[IdxC * RoomCenter.Num() + IdxA] = FMath::Min(AdjacencyList[IdxC * RoomCenter.Num() + IdxA], DistAC);
		AdjacencyList[IdxC * RoomCenter.Num() + IdxB] = FMath::Min(AdjacencyList[IdxC * RoomCenter.Num() + IdxB], DistBC);
	}
	
	// Prim Algorithm for MST
	TArray<bool> Visited;
	Visited.Init(false, RoomCenter.Num());
	TArray<int32> Parent;
	Parent.Init(-1, RoomCenter.Num());
	TArray<double> OpenList;
	OpenList.Init(TNumericLimits<double>::Max(), RoomCenter.Num());
	OpenList[0] = 0;
	for(int32 Idx = 0; Idx < RoomCenter.Num() - 1; ++Idx)
	{
		double MinCost = TNumericLimits<double>::Max();
		int32 MinIdx = -1;
		for(int i = 0; i < RoomCenter.Num(); ++i)
		{
			if(Visited[i])
				continue;
			if(OpenList[i] < MinCost)
			{
				MinCost = OpenList[i];
				MinIdx = i;
			}
		}
		Visited[MinIdx] = true;
		// Add new reachable edges to open list
		for(int i = 0; i < RoomCenter.Num(); ++i)
		{
			// Not reachable
			if(AdjacencyList[MinIdx * RoomCenter.Num() + i] == TNumericLimits<double>::Max() || Visited[i])
				continue;
			if(AdjacencyList[MinIdx * RoomCenter.Num() + i] < OpenList[i])
			{
				Parent[i] = MinIdx;
				OpenList[i] = AdjacencyList[MinIdx * RoomCenter.Num() + i];
			}
		}
	}

	// Then Perform A Star (Or maybe we can simply “rasterize” the line
	for(int Idx = 1; Idx < RoomCenter.Num(); ++Idx)
	{
		const auto ShortestPath = DungeonGenerationUtils::AStar(InGenerator, Data,
			Rooms[Parent[Idx]],
			Rooms[Idx]);
		TArray<FVector> CorridorPoints;
		
		for(const auto CurPoint : ShortestPath)
		{
			auto& CurTile = Data[CurPoint.X * InGenerator.GetLenY() + CurPoint.Y];
			if(Rooms[Idx].IsInside(CurPoint))
				continue;
			if(Rooms[Parent[Idx]].IsInside(CurPoint))
				break;
			
			CorridorPoints.Add(FVector(CurPoint.X, CurPoint.Y, 0.));
			CurTile = EDungeonType::Corridor;
		}
		
		InGenerator.PlaceCorridor(CorridorPoints);
	}

	// Choose a Room to Spawn Treasure Chest
	{
		const uint32 RandRoomIdx = FMath::RandRange(0, Rooms.Num() - 1);
		const FVector MinPoint(Rooms[RandRoomIdx].Min, 0.f);
		const FVector MaxPoint(Rooms[RandRoomIdx].Max, 0.f);
		const FBox SelectedRoom(MinPoint, MaxPoint);
		const FVector RandPoint = FMath::RandPointInBox(SelectedRoom);
		InGenerator.AddChest(RandPoint.X, RandPoint.Y);
		const int32 Idx = static_cast<int32>(RandPoint.X) * InGenerator.GetLenX() + static_cast<int32>(RandPoint.Y);
		Data[Idx] = EDungeonType::Treasure;
	}

	// Choose a Room to Spawn Portal
	{
		const uint32 RandRoomIdx = FMath::RandRange(0, Rooms.Num() - 1);
		const auto RandCenter = Rooms[RandRoomIdx].GetCenter().IntPoint();
		InGenerator.AddPortal(RandCenter.X, RandCenter.Y);
		Data[RandCenter.X * InGenerator.GetLenX() + RandCenter.Y] = EDungeonType::Portal;
	}
	
	return Data;
}