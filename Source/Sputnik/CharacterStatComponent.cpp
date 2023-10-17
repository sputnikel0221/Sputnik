// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStatComponent.h"
#include "SpGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UCharacterStatComponent::UCharacterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false; // Tick사용 하지 않음
	bWantsInitializeComponent = true;	// InitializeComponent를 실행시키려면 적용해야 함.
}


// Called when the game starts
void UCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

// 초기화문 - 시작시 한 번 실행
void UCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (GetOwner() != nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor : %s / Initialized CharacterStatComp"), *GetOwner()->GetName());
		SetNewLevel(Level);
		UE_LOG(LogTemp, Error, TEXT("Actor : %s / SetNewLevel Level is %d"), *GetOwner()->GetName(), Level);
	}
}


// Called every frame
void UCharacterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCharacterStatComponent::SetNewLevel(int32 NewLevel)
{
	auto SpGameInstance = Cast<USpGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	// 중요
	if (SpGameInstance == nullptr)
	{
		return;
	}

	CurrentStatData = SpGameInstance->GetCharacterStatData(NewLevel);

	if (nullptr != CurrentStatData)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s : Exist Level STAT +++++++"), *GetOwner()->GetName());
		Level = NewLevel;
		CurrentHP = CurrentStatData->MaxHP;

		// -
		SetHP(CurrentStatData->MaxHP);
		UE_LOG(LogTemp, Warning, TEXT("%s : Setted Level STAT !+++++++"), *GetOwner()->GetName());
	}
	else
	{
		// 디버그
		UE_LOG(LogTemp, Warning, TEXT("NO STAT DATA !+++++++"));
	}
}

void UCharacterStatComponent::SetDamage(float NewDamage)
{
	if (CurrentStatData != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("_-_-_-_- %f : SetDamage"), CurrentHP - NewDamage);
		SetHP(FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, CurrentStatData->MaxHP));
	}

	/*
	UE_LOG(LogTemp, Warning, TEXT("SetDamage"));

	if (CurrentStatData == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s : NoCurrentStatData==========="), *GetOwner()->GetName());	// 왜?
		return;
	}
		
	// CurrentHP - NewDamage의 범위를 지정.  0~MAX
	CurrentHP = FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, CurrentStatData->MaxHP);

	UE_LOG(LogTemp, Warning, TEXT("Actor : %s / Dealt : %f / CurrentHP : %f"), GetOwner(), NewDamage, CurrentHP);
	
	// 델리게이트 실행
	if (CurrentHP <= 0.0f)
	{
		OnHPIsZero.Broadcast();
	}
	*/
}

float UCharacterStatComponent::GetAttack()
{
	if (CurrentStatData == nullptr) return 0.0f;
	return CurrentStatData->Attack;
}


//
void UCharacterStatComponent::SetHP(float NewHP)
{
	//UE_LOG(LogTemp, Warning, TEXT("Enemy HP is %f"),NewHP);
	
	UE_LOG(LogTemp, Error, TEXT("(%f  ==  %f)"), CurrentHP, NewHP);

	CurrentHP = NewHP;
	OnHPChanged.Broadcast();

	UE_LOG(LogTemp, Error, TEXT("(%f)"), CurrentHP);

	if (CurrentHP < KINDA_SMALL_NUMBER)
	{
		CurrentHP = 0.0f;
		OnHPIsZero.Broadcast();
	}
}

float UCharacterStatComponent::GetHPRatio()
{
	if (CurrentStatData == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cant Get HP Ratio"));
		return 0.0f;
	}

	//UE_LOG(LogTemp, Error, TEXT("%f == %f PRERERERERERERERE"), CurrentHP,CurrentHP / CurrentStatData->MaxHP);
	return (CurrentStatData->MaxHP < KINDA_SMALL_NUMBER) ? 0.0f : (CurrentHP / CurrentStatData->MaxHP);
}

