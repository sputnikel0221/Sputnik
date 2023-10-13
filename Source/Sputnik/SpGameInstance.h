// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "SpGameInstance.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FSpCharacterStat : public FTableRowBase
{
	GENERATED_BODY();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 Level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float MaxHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float Attack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 DropExp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 NextExp;

	// Default Struct Value
	FSpCharacterStat() : Level(1), MaxHP(100.0f), Attack(10.0f), DropExp(10), NextExp(30) {}
};

UCLASS()
class SPUTNIK_API USpGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	USpGameInstance();
	
	virtual void Init() override;

	//GameInstance와 연동된 DT 구조체를 가져오는 함수
	FSpCharacterStat* GetCharacterStatData(int32 Level);

private:
	UPROPERTY()
		class UDataTable* SpCharacterStatTable;
};
