// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SPUTNIK_API UCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()


public:
	// Sets default values for this component's properties
	UCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	// Character Stat
public:
	void SetNewLevel(int32 NewLevel);
private:
	struct FSpCharacterStat* CurrentStatData = nullptr;
	UPROPERTY(EditInstanceOnly, Category = "CharacterStat", Meta = (AllowPrivateAccess = true))
		int32 Level = 1;
	UPROPERTY(Transient, VisibleInstanceOnly, Category = "CharacterStat", Meta = (AllowPrivateAccess = true))
		float CurrentHP;

	// Damage 피격
public:
	// CurrentHP에서 Damage만큼 Subtract
	void SetDamage(float NewDamage);
	// 스탯 중 Attack값만 가져옴
	float GetAttack();
	FOnHPIsZeroDelegate OnHpIsZero;
};
