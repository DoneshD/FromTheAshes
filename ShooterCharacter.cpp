// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterCharacter.h"
#include "Projectile.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "DystopianShooterGameModeBase.h"
#include "Components/ArrowComponent.h"

//Check if ready for inheritance
//YOU GOT TO WORK ON THIS!!!

// Sets default values
AShooterCharacter::AShooterCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = GetCapsuleComponent();

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile SpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(GetMesh());

	MaxTotalAmmo = 72;
	MaxClipAmmo = 6;
	TotalAmmo = 36;
	ClipAmmo = 6;
	ReloadTime = 2;
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	ADystopianShooterGameModeBase *GameMode = GetWorld()->GetAuthGameMode<ADystopianShooterGameModeBase>();
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveFoward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);

	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &AShooterCharacter::LookRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AShooterCharacter::LookUp);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Pressed, this, &AShooterCharacter::PullTrigger);
	PlayerInputComponent->BindAction(TEXT("Reload"), EInputEvent::IE_Pressed, this, &AShooterCharacter::ReloadTimeValid);
}

void AShooterCharacter::PullTrigger()
{

	FHitResult Hit;
	FVector ShotDirection;
	FVector End;

	bool bSuccess = TraceShot(Hit, ShotDirection, End);

	FVector Select = UKismetMathLibrary::SelectVector(Hit.Location, End, bSuccess);
	FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
	FVector SocketLocation = GetMesh()->GetSocketLocation(TEXT("BulletSocket"));
	FRotator LookRotation = UKismetMathLibrary::FindLookAtRotation(SocketLocation, Select);
	FTransform LookFire = UKismetMathLibrary::MakeTransform(SocketLocation, LookRotation);

	if (CanFire)
	{

		if (ClipAmmo > 0)
		{
			UGameplayStatics::SpawnEmitterAttached(MuzzleMist, ProjectileSpawnPoint, TEXT("BulletSocket"));
			AProjectile *Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, LookFire);
			Projectile->SetOwner(this);
			ClipAmmo = ClipAmmo - 1;
		}

		else if (TotalAmmo > 0)
		{
			ReloadReady = false;
			ReloadTimeValid();
			//It's not a bug, its a feature
		}
		else
		{
			TriggerOutOfAmmoPopUp();
		}

		CanFire = false;
		GetWorldTimerManager().SetTimer(FireHandle, this, &AShooterCharacter::FireRateValid, FireRate, true);
	}
}

void AShooterCharacter::ReloadTimeValid()
{
	ReloadReady = true;
	GetWorldTimerManager().SetTimer(ReloadHandle, this, &AShooterCharacter::ReloadGun, ReloadTime, true);
}

bool AShooterCharacter::TraceShot(FHitResult &Hit, FVector &ShotDirection, FVector &End)
{

	AController *OwnerController = GetController();
	if (OwnerController == nullptr)
		return false;

	FVector Location;
	FRotator Rotation;

	OwnerController->GetPlayerViewPoint(Location, Rotation);
	ShotDirection = -Rotation.Vector();

	End = Location + Rotation.Vector() * 50000.f;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	return GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToApply = FMath::Min(Health, DamageToApply);
	Health -= DamageToApply;

	return DamageToApply;
}

float AShooterCharacter::GetHealthPercent() const
{
	return Health / MaxHealth;
}

void AShooterCharacter::FireRateValid()
{
	CanFire = true;
}

void AShooterCharacter::ReloadGun()
{
	if (ClipAmmo != MaxClipAmmo && ReloadReady == true)
	{
		if (TotalAmmo - (MaxClipAmmo - ClipAmmo) >= 0)
		{
			TotalAmmo = TotalAmmo - (MaxClipAmmo - ClipAmmo);
			ClipAmmo = MaxClipAmmo;
		}
		else
		{
			ClipAmmo = ClipAmmo + TotalAmmo;
			TotalAmmo = 0;
		}
	}
	ReloadReady = false;
}

void AShooterCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AShooterCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void AShooterCharacter::LookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue);
}

void AShooterCharacter::LookRight(float AxisValue)
{
	AddControllerYawInput(AxisValue);
}

bool AShooterCharacter::IsDead() const
{
	return Health <= 0;
}
