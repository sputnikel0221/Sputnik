// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_EnemyShoot.generated.h"

/**
 * 
 */
UCLASS()
class SPUTNIK_API UBTTask_EnemyShoot : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_EnemyShoot();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
