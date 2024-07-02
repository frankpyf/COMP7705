// Fill out your copyright notice in the Description page of Project Settings.


#include "Room.h"

#include "CapstoneProject/Character/HeroCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
int32 ARoom::TileUnitLen = 100;

// Sets default values
ARoom::ARoom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	BoundingSplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Bounding Spline"), true);
	SetRootComponent(BoundingSplineComponent);
	BoundingSplineComponent->ClearSplinePoints();
}

// Called when the game starts or when spawned
void ARoom::BeginPlay()
{
	Super::BeginPlay();
}

void ARoom::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	const int32 InX = XLen * TileUnitLen;
	const int32 InY = YLen * TileUnitLen;

	// Not sure why it has to be in the constructor
	// Otherwise, no pcg wall is spawned
	BoundingSplineComponent->AddSplineLocalPoint(FVector::ZeroVector);
	BoundingSplineComponent->SetSplinePointType(BoundingSplineComponent->GetNumberOfSplinePoints() - 1, ESplinePointType::Linear);
	BoundingSplineComponent->AddSplineLocalPoint(FVector(InX, 0, 0));
	BoundingSplineComponent->SetSplinePointType(BoundingSplineComponent->GetNumberOfSplinePoints() - 1, ESplinePointType::Linear);
	BoundingSplineComponent->AddSplineLocalPoint(FVector( InX, InY,0));
	BoundingSplineComponent->SetSplinePointType(BoundingSplineComponent->GetNumberOfSplinePoints() - 1, ESplinePointType::Linear);
	BoundingSplineComponent->AddSplineLocalPoint(FVector(0, InY, 0));
	BoundingSplineComponent->SetSplinePointType(BoundingSplineComponent->GetNumberOfSplinePoints() - 1, ESplinePointType::Linear);
	
	BoundingSplineComponent->AddSplineLocalPoint(FVector::ZeroVector);
	BoundingSplineComponent->SetSplinePointType(BoundingSplineComponent->GetNumberOfSplinePoints() - 1, ESplinePointType::Linear);

	BoundingSplineComponent->SetClosedLoop(true);

	UBoxComponent* EnemySpawnerBox = NewObject<UBoxComponent>(this, UBoxComponent::StaticClass(), TEXT("EnemySpawnerBox"));
	EnemySpawnerBox->SetupAttachment(RootComponent);
	EnemySpawnerBox->AddRelativeLocation(FVector( InX / 2.f, InY / 2.f,0));
	EnemySpawnerBox->SetBoxExtent(FVector( InX / 2.f, InY / 2.f,0));
	EnemySpawnerBox->SetCollisionProfileName("OverlapOnlyPawn");
	EnemySpawnerBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	EnemySpawnerBox->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	EnemySpawnerBox->SetGenerateOverlapEvents(true);
	EnemySpawnerBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ARoom::SpawnEnemy);
	EnemySpawnerBox->RegisterComponent();
}