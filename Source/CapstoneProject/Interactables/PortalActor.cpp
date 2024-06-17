// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalActor.h"

bool APortalActor::bIsPreparing = true;

// Sets default values
APortalActor::APortalActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
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

// Called when the game starts or when spawned
void APortalActor::BeginPlay()
{
	Super::BeginPlay();
}
