// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SpEnemy.generated.h"

UCLASS()
class SPUTNIK_API ASpEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASpEnemy();
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class ASpProjectile> SpProjectileClass;

	void Shoot();

private:
	UPROPERTY(VisibleAnywhere, Category = "CharacterStat")
		class UCharacterStatComponent* EnemyStat;
	void Dead();

private:
	UFUNCTION()
		void OnHpIsZeroFunc();
};
