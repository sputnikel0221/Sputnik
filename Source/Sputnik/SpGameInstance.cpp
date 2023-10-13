// Fill out your copyright notice in the Description page of Project Settings.


#include "SpGameInstance.h"

USpGameInstance::USpGameInstance()
{
	// DT 매핑
	FString DTPath = TEXT("/Script/Engine.DataTable'/Game/CharacterData.CharacterData'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_SpCharacterStat(*DTPath);

	SpCharacterStatTable = DT_SpCharacterStat.Object;
}

void USpGameInstance::Init() // 게임 실행시 가장 먼저 시작 - 게임 앱의 초기화
{
	Super::Init();

	// 디버그
	UE_LOG(LogTemp, Error, TEXT("DT successed %f"), GetCharacterStatData(1)->Attack);
}

// Table에서 Level에 맞는 Row만을 찾아냄
FSpCharacterStat* USpGameInstance::GetCharacterStatData(int32 Level)
{
		return SpCharacterStatTable->FindRow<FSpCharacterStat>(*FString::FromInt(Level), TEXT(""));
}
