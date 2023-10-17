// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_DetectPlayer.generated.h"

//DECLARE_MULTICAST_DELEGATE(FIsDetectedDelegate);
/**
 * 
 */
UCLASS()
class SPUTNIK_API UBTService_DetectPlayer : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_DetectPlayer();
	//FIsDetectedDelegate IsDetected;

protected:
	// Tick
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
