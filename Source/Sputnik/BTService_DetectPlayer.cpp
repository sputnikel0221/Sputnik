// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_DetectPlayer.h"
#include "SpEnemyController.h"
#include "SpCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

UBTService_DetectPlayer::UBTService_DetectPlayer()
{
	NodeName = TEXT("Detect");
	Interval = 0.005;
}

void UBTService_DetectPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// 1. AIController에 빙의된 Pawn가져오기
	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControlledPawn) return;	

	// 2. World / Center / Radius 변수 값 채우기
	UWorld* World = ControlledPawn->GetWorld();
	FVector Center = ControlledPawn->GetActorLocation();
	float DetectRadius = 800.0f; // <- 6미터 = 탐색 범위
	if (nullptr == World) return;

	// 3. Overlap 결과 목록을 담을 TArray 구조체 선언 
	// + CollisionParam (탐색방법) 설정
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControlledPawn);

	// 4, OverlapMultiByChannel의 매개변수 채우기
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	// 5. 시각적 Debug 도형 그리기
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
	if (bResult)
	{
		for (auto OverlapResult : OverlapResults)
		{
			ASpCharacter* OverlappedCharacter = Cast<ASpCharacter>(OverlapResult.GetActor());
						
			
			if (OverlappedCharacter) //&& OverlappedCharacter->GetController()->IsPlayerController()
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("ChasedTarget"), OverlappedCharacter);
				//UE_LOG(LogTemp, Error, TEXT(" % s : IS OVERLAPPED"), *OverlappedCharacter->GetName());
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);

				// Player의 위치를 지속적으로 갱신
				// 아래줄 왜 주석했지?
				//OwnerComp.GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), OverlappedCharacter->GetActorLocation());
				OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("ChaseFlag"), true);
				//OwnerComp.GetAIOwner()->SetFocus(OverlappedCharacter, 0);

				float AttackDistance = FVector::Distance(OverlappedCharacter->GetActorLocation(), ControlledPawn->GetActorLocation());
				

				/*
				if (AttackDistance <= 400.0f)
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("AttackDistance"), true);
				}
				else
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("AttackDistance"), false);
				}
				*/

				//UE_LOG(LogTemp, Warning, TEXT("%d"), OwnerComp.GetBlackboardComponent()->GetValueAsBool(TEXT("AttackDistance")));
				return;
			}
			else
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("ChasedTarget"), nullptr);
				
				//UE_LOG(LogTemp, Error, TEXT("Chased is nullptr"));
				//OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("ChaseFlag"), false);
				//OwnerComp.GetAIOwner()->ClearFocus(0);
			}
		}
	}
	
}
