// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SpEnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SPUTNIK_API USpEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool bIsAiming;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = true))
		float playerDirection;

protected:
	void BeginPlay();

public:
	USpEnemyAnimInstance();
	void changeAming();
	bool getbAiming();

public:
	void PlayAttackMontage();
	float GetPlayerDirection();

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
		UAnimMontage* AttackMontage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
		UAnimMontage* RollMontage;
};
