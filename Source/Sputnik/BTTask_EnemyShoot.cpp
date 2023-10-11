// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_EnemyShoot.h"
#include "SpEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SpEnemy.h"

UBTTask_EnemyShoot::UBTTask_EnemyShoot()
{
}

EBTNodeResult::Type UBTTask_EnemyShoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ASpEnemy* SpEnemy = Cast<ASpEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	SpEnemy->Shoot();

	return EBTNodeResult::Succeeded;
}
