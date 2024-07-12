// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "CapstoneProject/Interfaces/CombatInterface.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UGameplayEffect;
class UCharacterBaseAttributeSet;
class AWeaponBase;
class UGameplayAbility;

UCLASS()
class CAPSTONEPROJECT_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	UFUNCTION(BlueprintCallable)
	bool IsAlive() const { return bAlive; }
	
	UFUNCTION(BlueprintCallable)
	void GiveAbility(TSubclassOf<UGameplayAbility> NewAbility);
	
	UFUNCTION(BlueprintCallable)
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }
	
	class UCharacterBaseAttributeSet const * GetAttributeSet() const { return BaseAttributeSet; }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Begin Combat Interface **/
	
	FVector GetAttackSocketLocation_Implementation() override;
	void OnDeath_Implementation() override;

	/** End Combat Interface **/

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	virtual void HandleDeath();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool DoMeleeAttack();
public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(BlueprintReadWrite)
	AWeaponBase* OwningWeapon;
	
	UPROPERTY()
	const UCharacterBaseAttributeSet* BaseAttributeSet;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

	UPROPERTY(BlueprintReadWrite)
	bool bAlive = true;

	virtual void InitializeDefaultAttributes() const;
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;
};
