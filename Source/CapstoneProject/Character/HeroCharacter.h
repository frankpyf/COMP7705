// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "HeroCharacter.generated.h"

struct FInputActionValue;
struct FOnAttributeChangeData;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedEvent, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangedEvent, float, NewMaxHealth);
/**
 * 
 */
UCLASS()
class CAPSTONEPROJECT_API AHeroCharacter : public ABaseCharacter
{
	GENERATED_BODY()

protected:
	/** Interaction Component **/
	UPROPERTY(BlueprintReadOnly)
	class UInteractionComponent* InteractionComponent;
	
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

	UPROPERTY(EditDefaultsOnly)
	float LockRange = 1000.f;


	UPROPERTY(BlueprintReadOnly)
	bool bWantToStrafe = false;

	bool bLockOnEnemy = false;
private:
	UPROPERTY()
	TObjectPtr<class AEnemyCharacter> FocusedEnemy = nullptr;
	
public:
	AHeroCharacter();

	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;
protected:
	void Move(const FInputActionValue&);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Zoom(const float& Delta);

	UFUNCTION(BlueprintCallable)
	void TryLockOn();

	UFUNCTION(BlueprintCallable)
	void StopLocking();

	void DoLockOn(float DeltaTime);

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
private:
	void InitAbilityActorInfo();

	void WantToStrafe();
	void StopStrafing();
};
