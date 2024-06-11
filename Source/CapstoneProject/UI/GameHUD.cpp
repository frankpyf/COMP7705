// Fill out your copyright notice in the Description page of Project Settings.


#include "GameHUD.h"
#include "Blueprint/UserWidget.h"

void AGameHUD::BeginPlay()
{
	Super::BeginPlay();
	if(IsValid(MainWidgetClass))
	{
		MainWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), MainWidgetClass);
		MainWidget->AddToViewport();
	}
}
