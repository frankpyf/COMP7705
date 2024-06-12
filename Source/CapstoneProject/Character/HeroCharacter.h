// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "HeroCharacter.generated.h"

struct FInputActionValue;
struct FOnAttributeChangeData;

/**
 * 
 */
UCLASS()
class CAPSTONEPROJECT_API AHeroCharacter : public ABaseCharacter
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedEvent, float, NewHealth);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangedEvent, float, NewMaxHealth);
	
public:
	AHeroCharacter();

	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;
protected:
	void Move(const FInputActionValue&);

	// Rotate Camera Spring Arm by 45 degrees
	void RotateCamera();

	void OnHealthChanged(const FOnAttributeChangeData&) const;
	void OnMaxHealthChanged(const FOnAttributeChangeData&) const;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
private:
	void InitAbilityActorInfo();

protected:
	/** MappingContext */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Input)
	class UInputMappingContext* DefaultMappingContext;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Input)
	class UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Input)
	UInputAction* RotateCameraAction;
	
	UPROPERTY(BlueprintAssignable, Category = "Abilities | Attribute")
	FOnHealthChangedEvent HealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Abilities | Attribute")
	FOnMaxHealthChangedEvent MaxHealthChanged;
};
