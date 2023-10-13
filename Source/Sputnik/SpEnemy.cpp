// Fill out your copyright notice in the Description page of Project Settings.


#include "SpEnemy.h"
#include "SpProjectile.h"
#include "CharacterStatComponent.h"
#include "SpEnemyAnimInstance.h"
#include "TimerManager.h"

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
}

void ASpEnemy::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	EnemyStat->OnHpIsZero.AddUObject(this, &ASpEnemy::OnHpIsZeroFunc);
}

// Called every frame
void ASpEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	UE_LOG(LogTemp, Warning, TEXT("Damage Taken"));
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	EnemyStat->SetDamage(Damage); // 1 현재-Damage 진행 / 2 Zero시, Delegate 실행
	return Damage;
}

