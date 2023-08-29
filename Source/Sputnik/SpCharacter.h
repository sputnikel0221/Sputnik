// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SpCharacter.generated.h"

UCLASS()
class SPUTNIK_API ASpCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASpCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 이동관련
protected:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void Aiming();
	void NotAiming();


public:
	void Shoot();

private:
	UPROPERTY(EditAnywhere)
		float MaxRange = 1000;
	UPROPERTY(VisibleAnywhere)
		USkeletalMeshComponent* SpMesh;

	UPROPERTY(EditAnywhere)
		UParticleSystem* MuzzleEffect;
	UPROPERTY(EditAnywhere)
		UParticleSystem* ImpactEffect;
	UPROPERTY(EditAnywhere)
		USoundBase* MuzzleSound;
	UPROPERTY(EditAnywhere)
		USoundBase* ImpactSound;


};
