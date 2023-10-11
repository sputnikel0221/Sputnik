// Fill out your copyright notice in the Description page of Project Settings.


#include "SpEnemy.h"
#include "SpProjectile.h"

// Sets default values
ASpEnemy::ASpEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASpEnemy::BeginPlay()
{
	Super::BeginPlay();

	this->bUseControllerRotationYaw = false;
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
			MuzzleRotation.Pitch += 30.0f;
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
			}

		}
	}
}

