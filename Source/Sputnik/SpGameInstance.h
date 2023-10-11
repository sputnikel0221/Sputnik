// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SpGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SPUTNIK_API USpGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	USpGameInstance();
	
	virtual void Init() override;

};
