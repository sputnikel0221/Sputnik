// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_SetSimpleValue.generated.h"

/**
 * 
 */
UCLASS()
class SPUTNIK_API UBTTask_SetSimpleValue : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	public:
		UBTTask_SetSimpleValue();

	protected:
		virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


};
