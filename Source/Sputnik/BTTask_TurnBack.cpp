// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TurnBack.h"
#include "SpEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TurnBack::UBTTask_TurnBack()
{

}

EBTNodeResult::Type UBTTask_TurnBack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	FRotator Rotation = OwnerComp.GetBlackboardComponent()->GetValueAsRotator(TEXT("EnemyStartRotation"));

	if (Rotation != OwnerComp.GetAIOwner()->GetPawn()->GetActorRotation())
	{
		OwnerComp.GetAIOwner()->GetPawn()->SetActorRotation(Rotation);
	}

	return EBTNodeResult::Succeeded;
}
