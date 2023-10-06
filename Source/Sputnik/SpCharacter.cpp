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

// Sets default values
ASpCharacter::ASpCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
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
			UE_LOG(LogTemp, Warning, TEXT("Lerp END"));
			bInterpolatingCamera = false;
		}
	}
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

void ASpCharacter::Shoot()
{
	if (!SpAnimInstance->getbAiming() || bIsRolling)
	{
		return;
	}
	UE_LOG(LogTemp, Error, TEXT("%d : %d"), SpAnimInstance->getbAiming(), bIsRolling);

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
		if (HitActor == nullptr) return;

		// HitActor를 Cast하고, 관련 피격 함수를 실행
	}
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




