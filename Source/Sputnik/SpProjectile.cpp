// Fill out your copyright notice in the Description page of Project Settings.


#include "SpProjectile.h"
#include "SpCharacter.h"
#include "Engine/DamageEvents.h" 
#include "GameFramework/ProjectileMovementComponent.h"
#include "TimerManager.h"

// Sets default values
ASpProjectile::ASpProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASpProjectile::BeginPlay()
{
	Super::BeginPlay();

	 UProjectileMovementComponent* ProjectileMovement = FindComponentByClass<UProjectileMovementComponent>();
	 ProjectileMovement->ProjectileGravityScale = 1.6f;
	 ProjectileMovement->InitialSpeed = 900.0f;
}

// Called every frame
void ASpProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// 소환 후, 방향에 맞춰 speed에 따라 발사.
void ASpProjectile::FireInDirection(const FVector& ShootDirection)
{
	UProjectileMovementComponent* ProjectileMovement = FindComponentByClass<UProjectileMovementComponent>();
	ProjectileMovement->Velocity = ShootDirection * ProjectileMovement->InitialSpeed;
}

void ASpProjectile::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, 
	bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);


	// Projectile이 부딪힌 객체가 Player라면,

	/*
	auto SpCharacter = Cast<ASpCharacter>(Other);

	if (SpCharacter != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Get Damage %f"), BulletAttack);


		FDamageEvent DamageEvent;
		// HitActor에게 Damage를 입힌다.
		SpCharacter->TakeDamage(BulletAttack, DamageEvent, nullptr, this);

		// Projectile을 파괴하거나 비활성화
		//SetActorHiddenInGame(true);
		//SetActorEnableCollision(false);

		
	}*/
}

void ASpProjectile::SetAttack(float Attack)
{
	BulletAttack = Attack;
}

void ASpProjectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	UE_LOG(LogTemp, Warning, TEXT("BULLET HIT :  %s"), *OtherActor->GetName());

	// Projectile이 부딪힌 객체가 Player라면,
	auto SpCharacter = Cast<ASpCharacter>(OtherActor);

	if (SpCharacter != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Get Damage %f"), BulletAttack);


		FDamageEvent DamageEvent;
		// HitActor에게 Damage를 입힌다.
		SpCharacter->TakeDamage(BulletAttack, DamageEvent, nullptr, this);

		// Projectile 비활성화
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
	}
}

// 실제로 Actor가 사라지는 것은 Enemy로부터 실행
void ASpProjectile::SetProjectileTimer()
{
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ASpProjectile::DestroyProjectile, 2.0f, false);
}

void ASpProjectile::DestroyProjectile()
{
	Destroy();
}

