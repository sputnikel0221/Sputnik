// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetSimpleValue.h"
#include "SpEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SetSimpleValue::UBTTask_SetSimpleValue()
{
}

EBTNodeResult::Type UBTTask_SetSimpleValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), true);
	return EBTNodeResult::Succeeded;
}
