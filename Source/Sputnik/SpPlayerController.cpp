// Fill out your copyright notice in the Description page of Project Settings.


#include "SpPlayerController.h"
#include "Blueprint/UserWidget.h"

void ASpPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UUserWidget* HUD = CreateWidget(this, HUDClass);
	if (HUD != nullptr)
	{
		HUD->AddToViewport();
	}
}

// Roll to direction
void ASpPlayerController::RollRotate(FRotator direction)
{
	SetControlRotation(direction);
}
