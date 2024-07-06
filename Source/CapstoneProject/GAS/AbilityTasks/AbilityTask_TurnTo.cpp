// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityTask_TurnTo.h"

#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"


void UAbilityTask_TurnTo::Activate()
{
	Super::Activate();
	bTickingTask = true;
	const FVector SourceLocation = GetAvatarActor()->GetActorLocation();
	const FVector TargetLocation(Location.X, Location.Y, SourceLocation.Z);
	DeltaRotation = UKismetMathLibrary::FindLookAtRotation(SourceLocation, TargetLocation);
}

void UAbilityTask_TurnTo::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);
	if(bFinished)
		return;
	if(ElapsedTime >= DurationOfTurning)
	{
		bFinished = true;
		Completed.Broadcast();
		return;
	}
	float Alpha = ElapsedTime / DurationOfTurning;
	auto LerpRotation = UKismetMathLibrary::RLerp(GetAvatarActor()->GetActorRotation(), DeltaRotation, Alpha, true);
	GetAvatarActor()->SetActorRotation(LerpRotation);
	ElapsedTime += DeltaTime;
}

UAbilityTask_TurnTo* UAbilityTask_TurnTo::TurnTo(UGameplayAbility* OwningAbility, const FVector& TargetLocation, float Duration)
{
	UAbilityTask_TurnTo* MyObj = NewAbilityTask<UAbilityTask_TurnTo>(OwningAbility);
	MyObj->Location = TargetLocation;
	MyObj->DurationOfTurning = Duration < 0.01f ? 0.01f : Duration;
	return MyObj;
}

void UAbilityTask_TurnTo::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	DOREPLIFETIME(UAbilityTask_TurnTo, Location);
	DOREPLIFETIME(UAbilityTask_TurnTo, DurationOfTurning);
}
