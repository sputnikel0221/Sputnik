// Fill out your copyright notice in the Description page of Project Settings.


#include "SpCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

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
	
	SpMesh = GetMesh();
}

// Called every frame
void ASpCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASpCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Aiming
	PlayerInputComponent->BindAction(TEXT("Aiming"), EInputEvent::IE_Pressed, this, &ASpCharacter::Aiming);
	PlayerInputComponent->BindAction(TEXT("Aiming"), EInputEvent::IE_Released, this, &ASpCharacter::NotAiming);

	// Jump / Shoot
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &ASpCharacter::Shoot);

	// Move
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ASpCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ASpCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
}

void ASpCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void ASpCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void ASpCharacter::Aiming()
{
	UE_LOG(LogTemp, Warning, TEXT("Aiming==="));
}

void ASpCharacter::NotAiming()
{
	UE_LOG(LogTemp, Warning, TEXT("NotAimingXXX"));
}

void ASpCharacter::Shoot()
{
	UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, SpMesh, TEXT("Muzzle_01"));
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, SpMesh, TEXT("Muzzle_01"));

	// 1 Get Controller's Location / Rotation
	AController* PlayerController = GetController();
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

