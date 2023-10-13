// Fill out your copyright notice in the Description page of Project Settings.


#include "SpAnimInstance.h"

void USpAnimInstance::BeginPlay()
{
	bIsAiming = false;
}

USpAnimInstance::USpAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Roll_Mont(TEXT("/Script/Engine.AnimMontage'/Game/AssetPacks/RollAnim_Montage.RollAnim_Montage'"));
	if (Roll_Mont.Succeeded())
	{
		RollMontage = Roll_Mont.Object;
	}
}

// 전환
void USpAnimInstance::changeAming()
{
	if (bIsAiming)
	{
		bIsAiming = false;
	}
	else
	{
		bIsAiming = true;
	}
}

bool USpAnimInstance::getbAiming()
{
	return bIsAiming;
}

void USpAnimInstance::setbIsDead()
{
	bIsDead = true;
}

// 직접 받은 RollMontage를 실행
void USpAnimInstance::PlayRollMontage()
{
	Montage_Play(RollMontage);
}

float USpAnimInstance::GetPlayerDirection()
{
	return playerDirection;
}
