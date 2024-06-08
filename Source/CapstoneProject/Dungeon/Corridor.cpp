// Fill out your copyright notice in the Description page of Project Settings.


#include "Corridor.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Engine/SplineMeshActor.h"

// Sets default values
ACorridor::ACorridor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CorridorSplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Corridor Spline"));
	SetRootComponent(CorridorSplineComponent);
	CorridorSplineComponent->ClearSplinePoints();

	AddPoint(GetActorLocation());
}

void ACorridor::AddPoint(const FVector& InPos)
{
	CorridorSplineComponent->AddSplineWorldPoint(InPos);
	CorridorSplineComponent->SetSplinePointType(CorridorSplineComponent->GetNumberOfSplinePoints()-1, ESplinePointType::Constant);
}

// Called when the game starts or when spawned
void ACorridor::BeginPlay()
{
	Super::BeginPlay();


	// TODO: Make this spline mesh work, because pcg doesn't work very well in this case
	for(int32 Idx = 0; Idx <= CorridorSplineComponent->GetNumberOfSplinePoints() - 2; ++Idx)
	{
		auto AddedComp = AddComponentByClass(USplineMeshComponent::StaticClass(), false, FTransform::Identity, false);
		GEngine->AddOnScreenDebugMessage(1, 10.0f, FColor::Red, TEXT("No"));
		if(auto SplineMeshComponent = Cast<USplineMeshComponent>(AddedComp))
		{
			GEngine->AddOnScreenDebugMessage(1, 10.0f, FColor::Red, TEXT("Added"));
			SplineMeshComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
			const auto StartLocation = CorridorSplineComponent->GetLocationAtSplinePoint(Idx, ESplineCoordinateSpace::Local);
			const auto EndLocation = CorridorSplineComponent->GetLocationAtSplinePoint(Idx+1, ESplineCoordinateSpace::Local);
			const auto StartTangent = CorridorSplineComponent->GetTangentAtSplinePoint(Idx, ESplineCoordinateSpace::Local);
			const auto EndTangent = CorridorSplineComponent->GetTangentAtSplinePoint(Idx+1, ESplineCoordinateSpace::Local);
			SplineMeshComponent->SetStartAndEnd(StartLocation, StartTangent, EndLocation, EndTangent);
		}
		// FinishAddComponent(AddedComp, false)
	}
}

// Called every frame
void ACorridor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

