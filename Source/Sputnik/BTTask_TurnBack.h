// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_TurnBack.generated.h"

/**
 * 
 */
UCLASS()
class SPUTNIK_API UBTTask_TurnBack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_TurnBack();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	
};
