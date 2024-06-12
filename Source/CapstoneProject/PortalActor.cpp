// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalActor.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
APortalActor::APortalActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot");
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	MeshComponent->SetupAttachment(GetRootComponent());
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PortalMeshAsset(TEXT("/Game/RougeLike/Assets/Scene/Portal/PortalThing"));
	if(PortalMeshAsset.Succeeded())
	{
		UStaticMesh* PortalMesh = PortalMeshAsset.Object;
		MeshComponent->SetStaticMesh(PortalMesh);
	}
}

void APortalActor::OnOverlap()
{
	if(!LevelsToLoad.IsEmpty())
	{
		if(bIsPreparing)
		{
			const auto RandLevelIdx = FMath::RandRange(0, LevelsToLoad.Num() - 1);
			const auto& LevelName = LevelsToLoad[RandLevelIdx];
			GEngine->AddOnScreenDebugMessage(1, 5, FColor::Red, FString("Level to load: ") + LevelName.ToString());
			FLatentActionInfo LatentInfo;
			UGameplayStatics::LoadStreamLevel(this, LevelName, true, true, LatentInfo);
		}
		else
		{
			
		}
	}
}

// Called when the game starts or when spawned
void APortalActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APortalActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

