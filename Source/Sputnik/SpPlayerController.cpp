// Fill out your copyright notice in the Description page of Project Settings.

#include "SpCharacter.h"
#include "SpPlayerController.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

void ASpPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// UUserWidget* 
	HUD = CreateWidget(this, HUDClass);
	if (HUD != nullptr)
	{
		HUD->AddToViewport();
	}

	ZoomOutCrosshair();

	UE_LOG(LogTemp, Warning, TEXT("=== PLAYERCONTROLLER ==="));
}

// Roll to direction
void ASpPlayerController::RollRotate(FRotator direction)
{
	SetControlRotation(direction);
}


void ASpPlayerController::ZoomInCrosshair()
{
	UTextBlock* C_Left = Cast<UTextBlock>(HUD->GetWidgetFromName(TEXT("C_left")));
	UTextBlock* C_Right = Cast<UTextBlock>(HUD->GetWidgetFromName(TEXT("C_right")));

	UCanvasPanelSlot* CL = Cast<UCanvasPanelSlot>(C_Left->Slot);
	UCanvasPanelSlot* CR = Cast<UCanvasPanelSlot>(C_Right->Slot);
	if (CL && CR)
	{
		CL->SetPosition(FVector2D(-20.0f, -2.5f));
		CR->SetPosition(FVector2D(20.0f, -2.5f));
	}
}

void ASpPlayerController::ZoomOutCrosshair()
{
	UTextBlock* C_Left = Cast<UTextBlock>(HUD->GetWidgetFromName(TEXT("C_left")));
	UTextBlock* C_Right = Cast<UTextBlock>(HUD->GetWidgetFromName(TEXT("C_right")));

	UCanvasPanelSlot* CL = Cast<UCanvasPanelSlot>(C_Left->Slot);
	UCanvasPanelSlot* CR = Cast<UCanvasPanelSlot>(C_Right->Slot);
	if (CL && CR)
	{
		CL->SetPosition(FVector2D(-50.0f, -2.5f));
		CR->SetPosition(FVector2D(50.0f, -2.5f));
	}
}




	/*
UUserWidget* C_Up = Cast<UUserWidget>(HUD->GetWidgetFromName(TEXT("C_up")));
UUserWidget* C_Down = Cast<UUserWidget>(HUD->GetWidgetFromName(TEXT("C_down")));
UUserWidget* C_Left = Cast<UUserWidget>(HUD->GetWidgetFromName(TEXT("C_left")));
UUserWidget* C_Right = Cast<UUserWidget>(HUD->GetWidgetFromName(TEXT("C_right")));
*/

	
	/*
	if (C_Up && C_Down && C_Left && C_Right)
	{
		UE_LOG(LogTemp, Warning, TEXT("=== YESYESYES CROSSHAIR ==="));

		C_Up->SetRenderTranslation(FVector2D(0.0f, -20.0f));
		C_Down->SetRenderTranslation(FVector2D(0.0f, 20.0f));
		C_Left->SetRenderTranslation(FVector2D(-20.0f, 0.0f));
		C_Right->SetRenderTranslation(FVector2D(20.0f, 0.0f));


		C_Up->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("=== No CROSSHAIR ==="));
	}
	
}
*/
