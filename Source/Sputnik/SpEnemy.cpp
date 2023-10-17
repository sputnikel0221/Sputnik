// Fill out your copyright notice in the Description page of Project Settings.


#include "SpEnemy.h"
#include "SpProjectile.h"
#include "CharacterStatComponent.h"
#include "SpEnemyAnimInstance.h"
#include "TimerManager.h"
#include "HPBarWidget.h"
#include "Components/WidgetComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SpEnemyController.h"
#include "SpEnemyAnimInstance.h"
#include "AIController.h"

// Sets default values
ASpEnemy::ASpEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EnemyStat = CreateDefaultSubobject<UCharacterStatComponent>(TEXT("SpEnemyStatStatComp"));
}

// Called when the game starts or when spawned
void ASpEnemy::BeginPlay()
{
	Super::BeginPlay();

	this->bUseControllerRotationYaw = false;

	UWidgetComponent* Widgetcomponent = Cast<UWidgetComponent>(GetComponentByClass(UWidgetComponent::StaticClass()));
	UHPBarWidget* HPBarWidget = Cast<UHPBarWidget>(Widgetcomponent->GetWidget());
	/*
	UHPBarWidget* HPBarWidget = nullptr;
	HPBarWidget = Cast<UHPBarWidget>(GetComponentByClass(UHPBarWidget::StaticClass()));
	*/

	UE_LOG(LogTemp, Error, TEXT("Enemy ============"))
	if (HPBarWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("Tex HP Bar Widget Component============"))
		HPBarWidget->BindCharacterStat(EnemyStat);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("NO HP Bar Widget Component============"))
	}
}

void ASpEnemy::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	EnemyStat->OnHPIsZero.AddUObject(this, &ASpEnemy::OnHpIsZeroFunc);
}

// Called every frame
void ASpEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!showHP && !isTakeDamage)
	{
		// 사라짐
		UWidgetComponent* Widgetcomponent = Cast<UWidgetComponent>(GetComponentByClass(UWidgetComponent::StaticClass()));
		UHPBarWidget* HPBarWidget = Cast<UHPBarWidget>(Widgetcomponent->GetWidget());

		HPBarWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		// 표시
		UWidgetComponent* Widgetcomponent = Cast<UWidgetComponent>(GetComponentByClass(UWidgetComponent::StaticClass()));
		UHPBarWidget* HPBarWidget = Cast<UHPBarWidget>(Widgetcomponent->GetWidget());

		HPBarWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

// Called to bind functionality to input
void ASpEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASpEnemy::Shoot()
{
	if (SpProjectileClass)
	{
		// 이거 필요 없을 듯
		FVector Camocation;
		FRotator CamRotation;
		GetActorEyesViewPoint(Camocation, CamRotation);

		// 
		FVector MuzzleLocation;
		FRotator MuzzleRotation;
		USkeletalMeshComponent* SkeletalMeshComponent = GetMesh();
		if (SkeletalMeshComponent)
		{
			MuzzleLocation = SkeletalMeshComponent->GetSocketLocation(TEXT("Muzzle_01"));
			MuzzleRotation = SkeletalMeshComponent->GetSocketRotation(TEXT("Muzzle_01"));
			// 포물선으로 쏘기 위함.
			MuzzleRotation.Pitch += 20.0f;
			MuzzleRotation.Yaw -= 8.0f;
		}

		// Skew the aim to be slightly upwards.
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			// Spawn the projectile at the muzzle.
			ASpProjectile* ProjectileInstance = World->SpawnActor<ASpProjectile>(SpProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (ProjectileInstance)
			{
				// Set the projectile's initial trajectory.
				FVector LaunchDirection = MuzzleRotation.Vector();
				ProjectileInstance->FireInDirection(LaunchDirection);
				
				// Enemy의 Attack을 Projectile의 공격력으로 설정
				if (EnemyStat != nullptr)
				{
					ProjectileInstance->SetAttack(EnemyStat->GetAttack());
				}

				ProjectileInstance->SetProjectileTimer();
			}

		}
	}
}

// Actor 자체를 World에서 없앤다.
void ASpEnemy::Dead()
{
	//SetActorEnableCollision(false); // Destroy되며 같이 사라짐
	Destroy();
}

// Enemy가 죽었을 시,
void ASpEnemy::OnHpIsZeroFunc()
{
	UE_LOG(LogTemp, Error, TEXT("===DIE==="));

	if (GetMesh() != nullptr)
	{
		auto SpEnemyAnim = Cast<USpEnemyAnimInstance>(GetMesh()->GetAnimInstance());
		SpEnemyAnim->setbIsDead();
	}

	// AIController의 빙의를 종료하여 BT도 종료 시킴
	AController* SpEnemyController = GetController();
	if (SpEnemyController)
	{
		SpEnemyController->UnPossess();
	}

	// 3초 뒤 Dead함수
	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, this, &ASpEnemy::Dead, 3.0f, false);
}


float ASpEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy Damage Taken"));
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	UE_LOG(LogTemp, Warning, TEXT("DamageAmount Is %f  / Damage Is %f "), DamageAmount, Damage);
	EnemyStat->SetDamage(Damage); // 1 현재-Damage 진행 / 2 Zero시, Delegate 실행


	// UI Visible 관련 변수
	isTakeDamage = true;
	showHP = true;
	
	// 1.5초 이후 showHP는 false가 된다.
	FTimerHandle showHPTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(showHPTimerHandle, this, &ASpEnemy::showHPFalse, 1.5f, false);

	return Damage;
}

void ASpEnemy::showHPFalse()
{
	showHP = false;

	// 2.5초 안에 대미지를 받아야 UI유지됨
	FTimerHandle isTakeDamageFalseTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(isTakeDamageFalseTimerHandle, this, &ASpEnemy::isTakeDamageFalse, 2.5f, false);
}

void ASpEnemy::isTakeDamageFalse()
{
	isTakeDamage = false;
}


