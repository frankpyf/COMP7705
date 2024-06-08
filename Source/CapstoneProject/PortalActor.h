// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PortalActor.generated.h"

UCLASS()
class CAPSTONEPROJECT_API APortalActor : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mesh", meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Levels", meta=(AllowPrivateAccess="true"))
	TArray<FName> LevelsToLoad;
protected:
	UPROPERTY(BlueprintReadWrite)
	bool bIsPreparing = true;
public:	
	// Sets default values for this actor's properties
	APortalActor();
	
	UFUNCTION(BlueprintCallable)
	void OnOverlap();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
