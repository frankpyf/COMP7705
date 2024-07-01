// Fill out your copyright notice in the Description page of Project Settings.


#include "Corridor.h"

#include "VectorUtil.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"

// Sets default values
ACorridor::ACorridor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	CorridorSplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Corridor Spline"));
	SetRootComponent(CorridorSplineComponent);
	CorridorSplineComponent->ClearSplinePoints();
	Tags.Add(FName("Corridor"));
}

void ACorridor::AddPoint(const FVector& InPos)
{
	CorridorSplineComponent->AddSplinePoint(InPos, ESplineCoordinateSpace::World);	
}

// Called when the game starts or when spawned
void ACorridor::BeginPlay()
{
	Super::BeginPlay();
}

void ACorridor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	ensure(PathMaterial);
	ensure(Mesh);
	for(int32 Idx = 0; Idx < CorridorSplineComponent->GetNumberOfSplineSegments(); ++Idx)
	{
		auto PathwayMesh = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());

		PathwayMesh->RuntimeVirtualTextures.Add(LandscapeRVT);
		PathwayMesh->SetStaticMesh(Mesh);
		PathwayMesh->SetMaterial(0, PathMaterial);
		PathwayMesh->SetMobility(EComponentMobility::Movable);
		PathwayMesh->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		PathwayMesh->RegisterComponent();
		PathwayMesh->AttachToComponent(CorridorSplineComponent, FAttachmentTransformRules::KeepRelativeTransform);

		PathwayMesh->bCastDynamicShadow = false;
		
		const auto StartLocation = CorridorSplineComponent->GetLocationAtSplinePoint(Idx, ESplineCoordinateSpace::Local);
		const auto EndLocation = CorridorSplineComponent->GetLocationAtSplinePoint(Idx+1, ESplineCoordinateSpace::Local);
		const auto StartTangent = CorridorSplineComponent->GetTangentAtSplinePoint(Idx, ESplineCoordinateSpace::Local);
		const auto EndTangent = CorridorSplineComponent->GetTangentAtSplinePoint(Idx+1, ESplineCoordinateSpace::Local);
		PathwayMesh->SetStartAndEnd(StartLocation, StartTangent, EndLocation, EndTangent, true);
		PathwayMesh->SetStartScale(FVector2D(3.f));
		PathwayMesh->SetEndScale(FVector2D(3.f));
	}
}