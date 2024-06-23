// Fill out your copyright notice in the Description page of Project Settings.


#include "FadeBase.h"

#include "Components/TimelineComponent.h"

// Sets default values
AFadeBase::AFadeBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("Default Scene Root"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Default Mesh");
	Mesh->SetupAttachment(GetRootComponent());

	// Timeline Stuff
	FadeTimeline = CreateDefaultSubobject<UTimelineComponent>("Fade Timeline");
	InterpFunc.BindUFunction(this,FName("TimelineFloatReturn"));

	OpacityFloat = CreateDefaultSubobject<UCurveFloat>("Timeline Curve");
	OpacityFloat->FloatCurve.UpdateOrAddKey(0.f, 1.f);
	OpacityFloat->FloatCurve.UpdateOrAddKey(.5f, 0.f);

	FadeTimeline->AddInterpFloat(OpacityFloat, InterpFunc, "Opacity");
}

void AFadeBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	OriginalMaterial = Mesh->GetMaterials();

	for(const auto Mat : OriginalMaterial)
	{
		FadingMaterial.Add(UMaterialInstanceDynamic::Create(Mat, this));
	}
}

void AFadeBase::FadeIn()
{
	// Fade to Opacity 0.x
	FadeTimeline->ReverseFromEnd();

	// Switch Back to Original Material
	for(int32 Idx = 0; Idx < OriginalMaterial.Num(); ++Idx)
	{
		Mesh->SetMaterial(Idx, OriginalMaterial[Idx]);
	}
}

void AFadeBase::FadeOut()
{
	// Switch To Dynamic MAterials
	for(int32 Idx = 0; Idx < FadingMaterial.Num(); ++Idx)
	{
		Mesh->SetMaterial(Idx, FadingMaterial[Idx]);
	}

	// Fade to Opacity 0.x
	FadeTimeline->PlayFromStart();
}

void AFadeBase::TimelineFloatReturn(const float Opacity)
{
	for(auto DynamicMat : FadingMaterial)
	{
		DynamicMat->SetScalarParameterValue("Opacity", Opacity);
	}
}
