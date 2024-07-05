// Fill out your copyright notice in the Description page of Project Settings.


#include "ComboWindow.h"

#include "CapstoneProject/Character/HeroCharacter.h"


void UComboWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
                               const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if(auto HeroCharacter = Cast<AHeroCharacter>(MeshComp->GetOwner()))
	{
		HeroCharacter->SetCanCombo(true);
		HeroCharacter->SetComboNotify(this);
	}
}

void UComboWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if(auto HeroCharacter = Cast<AHeroCharacter>(MeshComp->GetOwner()))
		HeroCharacter->SetCanCombo(false);
}
