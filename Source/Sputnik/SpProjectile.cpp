// Fill out your copyright notice in the Description page of Project Settings.


#include "SpProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

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

