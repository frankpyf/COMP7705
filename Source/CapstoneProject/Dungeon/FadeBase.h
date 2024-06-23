// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "FadeBase.generated.h"

class UTimelineComponent;

UCLASS()
class CAPSTONEPROJECT_API AFadeBase : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	TArray<UMaterialInterface*> OriginalMaterial;

	UPROPERTY(VisibleAnywhere)
	TArray<UMaterialInstanceDynamic*> FadingMaterial;

	UPROPERTY()
	UTimelineComponent* FadeTimeline;

	UPROPERTY()
	UCurveFloat* OpacityFloat;
	FOnTimelineFloat InterpFunc;
	
protected:
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Mesh;
	
public:	
	// Sets default values for this actor's properties
	AFadeBase();

	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION(BlueprintCallable)
	void FadeIn();

	UFUNCTION(BlueprintCallable)
	void FadeOut();
private:
	UFUNCTION()
	void TimelineFloatReturn(const float Opacity);
};
