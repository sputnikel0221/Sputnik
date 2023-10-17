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
	virtual void PostInitializeComponents() override;

public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

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
	void Roll();
	void BlockKey();

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
	UPROPERTY(EditAnywhere)
		USoundBase* CritMuzzleSound;
	UPROPERTY(EditAnywhere)
		UParticleSystem* CritMuzzleEffect;

	UPROPERTY()
		class USpAnimInstance* SpAnimInstance;
	UPROPERTY()
		class APlayerController* PlayerController;

	UFUNCTION()
		void OnRollMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
		void LerpCamera(FRotator End);
	FRotator EndRot;
	bool bInterpolatingCamera = false;

	// SpringArm Lerp
	UPROPERTY()
		class USpringArmComponent* SpArm;
	UPROPERTY()
		float DeltaSec;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true), Category = "SpringArm")
		float InterpSpeed = 0.5f;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true), Category = "SpringArm")
		float AArmLen = 150.0f;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true), Category = "SpringArm")
		float BArmLen = 80.0f;

	float BPMovementAngle;

	bool bIsRolling = false;
	bool bBlockRolling = false;

	// Actor Component
	UPROPERTY(VisibleAnywhere, Category = "CharacterStat")
		class UCharacterStatComponent* CharacterStat;

	void Dead();


	// Delegate용 함수들
private:
	UFUNCTION()
		void OnHpIsZeroFunc();

private:
	float PlayerAttackTiming = 3.0f;
	float PlayerAttackBar = PlayerAttackTiming;
	float PlayerAttackRestore = 0.02f;
public:
	UFUNCTION(Blueprintcallable)
		float GetAttackBarRatio();
};
