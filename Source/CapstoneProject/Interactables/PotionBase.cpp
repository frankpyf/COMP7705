// Fill out your copyright notice in the Description page of Project Settings.


#include "PotionBase.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "CapstoneProject/CapstoneProjectPlayerController.h"
#include "CapstoneProject/Character/HeroCharacter.h"

// Sets default values
APotionBase::APotionBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void APotionBase::Interact_Implementation(AActor* InstigatorPawn)
{
	Super::Interact_Implementation(InstigatorPawn);
	if(auto HeroCharacter = Cast<AHeroCharacter>(InstigatorPawn))
	{
		auto PC = CastChecked<ACapstoneProjectPlayerController>(HeroCharacter->GetController());

		if(PC->AddItem(ItemType, AddAmount))
		{
			HeroCharacter->GiveAbility(AbilityToGive);
			Destroy();
		}
	}
}

void APotionBase::ApplyEffectToTarget(AActor* Target)
{
	if(auto TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target))
	{
		auto EffectContext = TargetASC->MakeEffectContext();
		EffectContext.AddSourceObject(this);
		check(EffectToApply)
		auto SpecHandle = TargetASC->MakeOutgoingSpec(EffectToApply, 1.f, EffectContext);
		TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

