// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "InputActionValue.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "GameFramework/Character.h"
#include "CapstoneProjectCharacter.generated.h"

struct FOnAttributeChangeData;
struct FInputActionValue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedEvent, float, NewHealth);

UCLASS(Blueprintable)
class ACapstoneProjectCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* DashAction;
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category= Abilities)
	UAbilitySystemComponent* AbilitySystem;

	UPROPERTY(BlueprintAssignable, Category = "Abilities | Attribute")
	FOnHealthChangedEvent HealthChanged;
protected:
	UPROPERTY()
	const class UCharacterBaseAttributeSet* BaseAttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Abilities)
	TArray<TSubclassOf<UGameplayAbility>> CharacterAbilities;
public:
	ACapstoneProjectCharacter();

	virtual void BeginPlay() override;
	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	virtual void Destroyed() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystem; }

	UFUNCTION(BlueprintCallable, Category = "Ability System")
	void GrantAbility(TSubclassOf<UGameplayAbility> Ability, int32 Level, int32 InputCode);
	UFUNCTION(BlueprintCallable, Category = "Ability System")
	void GrantAbilities(const TArray<TSubclassOf<UGameplayAbility>> Abilities);

	UFUNCTION(BlueprintCallable, Category = "Ability System")
	float GetHealth() const;
	
	UFUNCTION(BlueprintCallable, Category = "Ability System")
	float GetMaxHealth() const;

	void OnHealthChanged(const FOnAttributeChangeData&) const;
	void OnMoveSpeedChanged(const FOnAttributeChangeData&) const;
	void OnDead() const;
protected:
	void Move(const FInputActionValue&);
	UFUNCTION(BlueprintCallable)
	void RotateCamera();
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
};

