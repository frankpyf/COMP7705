// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComponent.h"

#include "CapstoneProject/Character/HeroCharacter.h"
#include "CapstoneProject/Interfaces/InteractInterface.h"

// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// Only tick local component
	if(const auto OwnerPawn = Cast<APawn>(GetOwner()))
	{
		if(!OwnerPawn->IsLocallyControlled())
			PrimaryComponentTick.bCanEverTick = false;
	}
}


// Called every frame
void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FindBestInteract();
}

void UInteractionComponent::DoInteract()
{
	ServerInteract(FocusedActor);
}

void UInteractionComponent::FindBestInteract()
{
	if(const auto OwnerCharacter = Cast<AHeroCharacter>(GetOwner()))
	{
		const auto OwnerController = OwnerCharacter->GetController<APlayerController>();
		FHitResult Hit;
		OwnerController->GetHitResultUnderCursor(ECC_Visibility, false, Hit);
		LastActor = FocusedActor;
		
		// Reset Focused Actor
		FocusedActor = nullptr;
		MouseWorldLocation = GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector();
		
		if(Hit.bBlockingHit)
		{
			// Find Interactable Actor
			if(const auto HitActor = Hit.GetActor())
			{
				if(HitActor->Implements<UInteractInterface>())
				{
					if(FVector::Dist(HitActor->GetActorLocation(), GetOwner()->GetActorLocation()) < 200.f)
						FocusedActor = HitActor;
				}
			}
			// Find Mouse Location in World Space
			MouseWorldLocation = Hit.Location;
		}

		if(LastActor != nullptr)
		{
			// Cancel Highlight of Last Actor
		}
		if(FocusedActor != nullptr)
		{
			// Highlight Focused Actor, if any
			IInteractInterface::Execute_Highlight(FocusedActor);
		}
	}
}

void UInteractionComponent::ServerInteract_Implementation(AActor* InFocusedActor)
{
	if(InFocusedActor == nullptr)
		return;
	IInteractInterface::Execute_Interact(InFocusedActor, GetOwner());
}

