// Fill out your copyright notice in the Description page of Project Settings.


#include "SpEnemyController.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h" 
#include "BehaviorTree/BlackboardComponent.h"
#include "TimerManager.h"
#include "SpCharacter.h"

void ASpEnemyController::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (EnemyBT != nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("RunBT"));
		RunBehaviorTree(EnemyBT);

		// 시작 위치를 저장
		GetBlackboardComponent()->SetValueAsVector(TEXT("EnemyStartLocation"), GetPawn()->GetActorLocation());
		GetBlackboardComponent()->SetValueAsRotator(TEXT("EnemyStartRotation"), GetPawn()->GetActorRotation());
	}
}

void ASpEnemyController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//bool bIsChase =  GetBlackboardComponent()->GetValueAsBool(TEXT("ChaseFlag"));

	auto ChasedTarget = GetBlackboardComponent()->GetValueAsObject(TEXT("ChasedTarget"));

	if(ChasedTarget != nullptr)
	{
		GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
		GetBlackboardComponent()->SetValueAsVector(TEXT("LastPlayerLocation"), PlayerPawn->GetActorLocation());
	}
	else
	{
		GetBlackboardComponent()->ClearValue(TEXT("PlayerLocation"));
	}
}

ASpEnemyController::ASpEnemyController()
{
	// 굳이 쓸 필요 없다.
	//RepeatInterval = 3.0f;
}

void ASpEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	APawn* ControlledPawn = Cast<APawn>(InPawn);

	/*
	GetWorld()->GetTimerManager().SetTimer(
		RepeatTimerHandle,
		this,
		RepeatInterval
	);*/

}

void ASpEnemyController::OnUnPossess()
{
	Super::OnUnPossess();

}
