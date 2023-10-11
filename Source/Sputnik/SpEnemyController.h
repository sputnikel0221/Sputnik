// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "SpEnemyController.generated.h"

/**
 * 
 */
UCLASS()
class SPUTNIK_API ASpEnemyController : public AAIController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

private:
	APawn* PlayerPawn;

private:
	UPROPERTY(EditAnywhere)
		float DetectRadius = 200;
	UPROPERTY(EditAnywhere)
		class UBehaviorTree* EnemyBT;

public:
	ASpEnemyController();
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
private:
	FTimerHandle RepeatTimerHandle;
	float RepeatInterval;
};
