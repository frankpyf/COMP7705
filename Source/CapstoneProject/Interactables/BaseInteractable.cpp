#include "BaseInteractable.h"

void ABaseInteractable::Interact_Implementation(AActor* InstigatorPawn)
{
	
}

void ABaseInteractable::Highlight_Implementation()
{
	DrawDebugSphere(GetWorld(), GetActorLocation(), 50, 30, FColor::Red);
}
