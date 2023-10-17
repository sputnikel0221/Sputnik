// Fill out your copyright notice in the Description page of Project Settings.


#include "SpCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "SpAnimInstance.h"
#include "GameFramework/SpringArmComponent.h"
#include "SpPlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "CharacterStatComponent.h"
#include "Engine/DamageEvents.h" 
#include "SpEnemy.h"

// Sets default values
ASpCharacter::ASpCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 오류가 발생하는 코드 왜? - 생성자에서만 사용 가능
	CharacterStat = CreateDefaultSubobject<UCharacterStatComponent>(TEXT("SpCharacterStatComp"));
}

// Called when the game starts or when spawned
void ASpCharacter::BeginPlay()
{
	Super::BeginPlay();

	// get Current Mesh
	SpMesh = GetMesh();

	// get Current SpringArm
	SpArm = this->FindComponentByClass<USpringArmComponent>();

	// 분리 설정
	this->bUseControllerRotationYaw = true;
	//SpArm->bUsePawnControlRotation = false; // 추가

}

void ASpCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Find AnimInstance from Mesh
	SpAnimInstance = Cast<USpAnimInstance>(GetMesh()->GetAnimInstance());
	SpAnimInstance->OnMontageEnded.AddDynamic(this, &ASpCharacter::OnRollMontageEnded);

	// GetPlayerController 
	PlayerController = Cast<APlayerController>(GetController());

	// Delegate 실행 == CharacterComponent의 델리게이트가 실행되면 실행될 것을 미리 엮어 둔다.
	CharacterStat->OnHPIsZero.AddUObject(this, &ASpCharacter::OnHpIsZeroFunc);
}

// 얼마나 데미지가 들어가는지 확인하는 함수
float ASpCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, 
	AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	CharacterStat->SetDamage(Damage); // 1 현재-Damage 진행 / 2 Zero시, Delegate 실행
	return Damage;
}

// Called every frame
void ASpCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 1 - Lerp Camera with Tick
	if (bInterpolatingCamera)
	{
		// StartRot이 필요가 없다.
		PlayerController = Cast<APlayerController>(GetController());
		if (PlayerController == nullptr) return;

		FRotator CurrentCameraRotation = PlayerController->GetControlRotation();

		// 현재 카메라 회전값을 목표 회전값으로 보간.
		FRotator NewCameraRotation = FMath::RInterpTo(CurrentCameraRotation, EndRot, DeltaTime, 3.0f); // InterpolationSpeed

		// 보간된 회전값을 카메라에 설정.
		PlayerController->SetControlRotation(NewCameraRotation);

		// 목표 회전값에 도달하면 보간 상태를 종료.
		if (CurrentCameraRotation.Equals(EndRot, 0.5f))	// 0.2의 각도까지 오차범위
		{
			//UE_LOG(LogTemp, Warning, TEXT("Lerp END"));
			bInterpolatingCamera = false;
		}
	}

	// Progress Bar
	PlayerAttackBar = FMath::Clamp<float>(PlayerAttackBar + PlayerAttackRestore, 0.0f, PlayerAttackTiming);
}

// Called to bind functionality to input
void ASpCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Aiming
	PlayerInputComponent->BindAction(TEXT("Aiming"), EInputEvent::IE_Pressed, this, &ASpCharacter::Aiming);
	PlayerInputComponent->BindAction(TEXT("Aiming"), EInputEvent::IE_Released, this, &ASpCharacter::NotAiming);

	// Jump / Shoot / Roll
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &ASpCharacter::Shoot);
	PlayerInputComponent->BindAction(TEXT("Roll"), EInputEvent::IE_Pressed, this, &ASpCharacter::Roll);

	// Move
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ASpCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ASpCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
}

void ASpCharacter::MoveForward(float AxisValue)
{
	//if (!bIsRolling) - 사용하면 부자연.
	AddMovementInput(GetActorForwardVector() * AxisValue);
	LerpCamera(GetCapsuleComponent()->GetComponentRotation());
}

void ASpCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
	LerpCamera(GetCapsuleComponent()->GetComponentRotation());
}

void ASpCharacter::Aiming()
{
	if (bIsRolling)
	{
		UE_LOG(LogTemp, Error, TEXT("Aiming - Block"));
		return;
	}

	ASpPlayerController* PController = Cast<ASpPlayerController>(GetController());
	PController->ZoomInCrosshair();

	UE_LOG(LogTemp, Warning, TEXT("Aiming"));
	DeltaSec = GetWorld()->GetDeltaSeconds();

	if (SpAnimInstance == nullptr)	return;

	if (!SpAnimInstance->getbAiming())
	{
		SpAnimInstance->changeAming();
	}

	if (SpArm == nullptr)	return;
	//SpArm->TargetArmLength = FMath::FInterpTo(BArmLen, AArmLen, DeltaSec, InterpSpeed);
}

void ASpCharacter::NotAiming()
{
	if (bIsRolling)
	{
		UE_LOG(LogTemp, Error, TEXT("NotAiming - Block"));
		return;
	}

	ASpPlayerController* PController = Cast<ASpPlayerController>(GetController());
	PController->ZoomOutCrosshair();

	DeltaSec = GetWorld()->GetDeltaSeconds();

	if (SpAnimInstance == nullptr)		return;

	if (SpAnimInstance->getbAiming())
	{
		SpAnimInstance->changeAming();
	}


	if (SpArm == nullptr)	return;
	//SpArm->TargetArmLength = FMath::FInterpTo(AArmLen, BArmLen, DeltaSec, InterpSpeed);
}

void ASpCharacter::Roll()
{
	if (bIsRolling || bBlockRolling)
	{
		UE_LOG(LogTemp, Error, TEXT("Rolling - Block"));
		return;
	}

	bIsRolling = true;
	bBlockRolling = true;

	// 회전 고정 해제
	this->bUseControllerRotationYaw = false;

	if (SpAnimInstance == nullptr)		return;
	//UE_LOG(LogTemp, Error, TEXT("%f"), SpAnimInstance->GetPlayerDirection());

	// PlayerDirection값을 rotator로 적용, 플레이어를 돌림
	FRotator ActorRotator = GetActorRotation();
	FRotator rotator = { ActorRotator.Pitch, ActorRotator.Yaw + SpAnimInstance->GetPlayerDirection(), ActorRotator.Roll };
	this->SetActorRelativeRotation(rotator);

	// AnimInstance Play Montage 
	SpAnimInstance->PlayRollMontage();
}

void ASpCharacter::BlockKey()
{
	this->EnableInput(PlayerController);
	bBlockRolling = false;
}

float ASpCharacter::GetAttackBarRatio()
{
	return PlayerAttackBar / PlayerAttackTiming;
}

void ASpCharacter::Shoot()
{
	if (!SpAnimInstance->getbAiming() || bIsRolling)
	{
		return;
	}
	//UE_LOG(LogTemp, Error, TEXT("%d : %d"), SpAnimInstance->getbAiming(), bIsRolling);

	// Muzzle Effect & Sound
	UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, SpMesh, TEXT("Muzzle_01"));
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, SpMesh, TEXT("Muzzle_01"));

	// 1 Get Controller's Location / Rotation
	if (PlayerController == nullptr) return;

	FVector Location;
	FRotator Rotation;
	PlayerController->GetPlayerViewPoint(Location, Rotation);

	// 2 make ShootDirection / End via Location / Rotation
	FVector ShootDirection = -Rotation.Vector();	// 피격 이펙트에 쓰임
	FVector End = Location + Rotation.Vector() * 3000;

	// 3 LineTrace with Ignores
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	FHitResult Hit;
	bool bResult = GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1, Params);

	// 4 if Hit is Success do
	if (bResult)
	{
		DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Red, true);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, ShootDirection.Rotation());
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ImpactSound, Hit.Location);

		AActor* HitActor = Hit.GetActor();
		if (HitActor == nullptr)
		{
			PlayerAttackBar = 0.0f;
			return;
		}

		ASpEnemy* EnemyActor = Cast<ASpEnemy>(HitActor);
		if (EnemyActor == nullptr)
		{
			PlayerAttackBar = 0.0f;
			return;
		}

		// 플레이어가 공격한 Actor에 대해서 TakeDamage 실행
		FDamageEvent DamageEvent;
		// HitActor에게 Damage를 입힌다. GetAttackBarRatio를 Attack에 곱함

		// 크리티컬
		float Attack_x = GetAttackBarRatio();
		if (0.66 <= Attack_x && Attack_x <= 0.74)
		{
			UGameplayStatics::SpawnSoundAttached(CritMuzzleSound, SpMesh, TEXT("Muzzle_01"));
			UGameplayStatics::SpawnEmitterAttached(CritMuzzleEffect, SpMesh, TEXT("Muzzle_01"));
			Attack_x = 1.5f;
			UE_LOG(LogTemp, Error, TEXT("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
		}

		UE_LOG(LogTemp, Warning, TEXT("AttackRatio Is %f / Final Damage Is %f"), GetAttackBarRatio(), CharacterStat->GetAttack() * Attack_x);

		EnemyActor->TakeDamage(CharacterStat->GetAttack() * Attack_x, DamageEvent, GetController(), this);
		UE_LOG(LogTemp, Warning, TEXT("Hit Actor : % s"), *HitActor->GetName());
		// HitActor를 Cast하고, 관련 피격 함수를 실행
	}

	// AttackBar
	PlayerAttackBar = 0.0f;
}

void ASpCharacter::OnRollMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	LerpCamera(GetCapsuleComponent()->GetComponentRotation());

	// 설정 원래대로
	this->bUseControllerRotationYaw = true;
	bIsRolling = false;

	// 구르기 중 구르기 방지
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ASpCharacter::BlockKey, 0.2f, false);
}

void ASpCharacter::LerpCamera(FRotator End)
{
	EndRot = End;
	bInterpolatingCamera = true;
}

// Delegate1
void ASpCharacter::OnHpIsZeroFunc()
{
	UE_LOG(LogTemp, Error, TEXT("===DIE==="));

	if (GetMesh() != nullptr)
	{
		SpAnimInstance->setbIsDead();
	}

	// AIController의 빙의를 종료하여 BT도 종료 시킴
	AController* SpController = GetController();
	if (SpController)
	{
		//SpController->UnPossess();
	}

	// 3초 뒤 Dead함수
	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, this, &ASpCharacter::Dead, 3.0f, false);
}

void ASpCharacter::Dead()
{
	Destroy();
}



