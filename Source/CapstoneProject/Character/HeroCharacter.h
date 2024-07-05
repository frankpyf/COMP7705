// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "GameplayEffectTypes.h"
#include "HeroCharacter.generated.h"

struct FInputActionValue;
struct FOnAttributeChangeData;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangedSignature, float, NewMaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStaminaChangeSignature, float, NewStamina);
/**
 * 
 */
UCLASS()
class CAPSTONEPROJECT_API AHeroCharacter : public ABaseCharacter
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<class AEnemyCharacter> FocusedEnemy = nullptr;
protected:
	/** Interaction Component **/
	UPROPERTY(BlueprintReadOnly)
	class UInteractionComponent* InteractionComponent;
	
	UPROPERTY(BlueprintReadOnly)
	class UComboWindow* ComboNotify;
	
	/** MappingContext */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Input)
	class UInputMappingContext* DefaultMappingContext;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Input)
	class UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Input)
	UInputAction* RotateCameraAction;
	
	UPROPERTY(BlueprintAssignable, Category = "Abilities | Attribute")
	FOnHealthChangedSignature HealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Abilities | Attribute")
	FOnMaxHealthChangedSignature MaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Abilities | Attribute")
	FOnStaminaChangeSignature StaminaChanged;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UGameplayEffect> StaminaRecoverEffect;
	FActiveGameplayEffectHandle ActiveStaminaRecoverEffect;

	UPROPERTY(EditDefaultsOnly)
	float LockRange = 1000.f;

	UPROPERTY(BlueprintReadOnly)
	bool bComboWindow = false;
	
	bool bExhausted = false;
	bool bLockOnEnemy = false;
public:
	AHeroCharacter();

	void SetComboNotify(UComboWindow*);
	void SetCanCombo(bool NewFlag);

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

	UFUNCTION(BlueprintCallable)
	bool IsTired() const { return bExhausted; }

	void DoLockOn(float DeltaTime);

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
private:
	void InitAbilityActorInfo();

	void WantToStrafe() const;
	void StopStrafing() const;
};
