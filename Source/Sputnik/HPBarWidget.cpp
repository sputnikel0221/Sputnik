// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterStatComponent.h"
#include "HPBarWidget.h"
#include "Components/ProgressBar.h"

void UHPBarWidget::BindCharacterStat(UCharacterStatComponent* NewCharacterStat)
{
	if (NewCharacterStat == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("NO Stat Comp"));
	}

	CurrentCharacterStat = NewCharacterStat;
	CurrentCharacterStat->OnHPChanged.AddUObject(this, &UHPBarWidget::UpdateHPWidget);
	UE_LOG(LogTemp, Warning, TEXT("= Binded DELEGATE ="));
}

void UHPBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HPBar")));

	if (HPProgressBar == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("NO HP Bar On Widget"));
	}

	UpdateHPWidget();
}

void UHPBarWidget::UpdateHPWidget()
{
	UE_LOG(LogTemp, Warning, TEXT("= START DELEGATE ="));

	if (CurrentCharacterStat.IsValid()) // is valiud는 뭘까 / 약포인터라?
	{
		if (HPProgressBar != nullptr)
		{
			HPProgressBar->SetPercent(CurrentCharacterStat->GetHPRatio());
			UE_LOG(LogTemp, Warning, TEXT("UpdateHPWidget +++ %f"), CurrentCharacterStat->GetHPRatio());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("NO HPProgressBar"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NO CurrentCharacterStat")); // 왜 없을까
	}
}
