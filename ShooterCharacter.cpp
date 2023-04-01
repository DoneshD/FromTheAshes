// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterCharacter.h"
#include "Projectile.h"
#include "ChargedProjectile.h"
#include "AugmentOrb.h"
#include "OrbState.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimSequence.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/WorldSettings.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "InputCoreTypes.h"
#include "TimerManager.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "DystopianShooterGameModeBase.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = GetCapsuleComponent();


	MaxTotalAmmo = 196;
	MaxClipAmmo = 24;
	TotalAmmo = 156;
	ClipAmmo = 24;
	ReloadTime = 2;

	GetCharacterMovement()->MaxWalkSpeed = 800;
	
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	ADystopianShooterGameModeBase *GameMode = GetWorld()->GetAuthGameMode<ADystopianShooterGameModeBase>();
}

// Called every frames
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//TODO: Fix me
	GetCharacterMovement()->MaxWalkSpeed = 800;

	// Chargeshot
	if (bRightClickIsPressed)
	{
		fRightClickTime += DeltaTime;
		
		if(!bAugmentReady)
			UGameplayStatics::SpawnEmitterAttached(ChargingMist, GetMesh(), TEXT("BulletSocket"));

		if(bAugmentReady)
		{
			UGameplayStatics::SpawnEmitterAttached(UlimateShotCharging, GetMesh(), TEXT("BulletSocket"));
		}
	}

	//Augment Charge
	if (bAugmentCharge && !bAugmentReady)
	{
		fAugmentChargeTime += DeltaTime;
		CanFire = false;
		UGameplayStatics::SpawnEmitterAttached(AugmentParticles, GetMesh(), TEXT("AugmentSocket"));
		if (fAugmentChargeTime > 1.0)
		{
			bAugmentReady = true;
			fAugmentChargeTime = 0.f;
		}
	}
	if (bAugmentReady || !bAugmentCharge)
	{
		StopAnimMontage(AugmentAnim);
	}
	if (bAugmentReady)
	{
		UGameplayStatics::SpawnEmitterAttached(AugmentChargedParticles, GetMesh(), TEXT("AugmentSocket"));
		GetCharacterMovement()->MaxWalkSpeed = 1200;
	}
	
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

	PlayerInputComponent->BindAction(TEXT("Charging"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Charge);
	PlayerInputComponent->BindAction(TEXT("Charging"), EInputEvent::IE_Released, this, &AShooterCharacter::ChargeTime);

	PlayerInputComponent->BindAction(TEXT("Grenade"), EInputEvent::IE_Released, this, &AShooterCharacter::ThrowGrenade);

	PlayerInputComponent->BindAction(TEXT("Augment"), EInputEvent::IE_Pressed, this, &AShooterCharacter::AugmentCharge);
	PlayerInputComponent->BindAction(TEXT("Augment"), EInputEvent::IE_Released, this, &AShooterCharacter::AugmentRelease);

	PlayerInputComponent->BindAction(TEXT("Shockwave"), EInputEvent::IE_Released, this, &AShooterCharacter::ShockwavePressed);
}


void AShooterCharacter::ResetTimeDilation()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1);
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

float AShooterCharacter::GetHealthPercent() const
{
	return Health / MaxHealth;
}

void AShooterCharacter::FireRateValid()
{
	if (ReloadReady)
		CanFire = true;	
}

// Select a random vector between the left and right vectors
FVector AShooterCharacter::GetRandomVectorBetween(FVector LeftVector, FVector RightVector)
{
    float Alpha = FMath::RandBool() ? 1.0f : 0.0f;
    return FMath::Lerp(LeftVector, RightVector, Alpha);
}

bool AShooterCharacter::GetRandomBoolBetween(bool ShouldDodge, bool DontDodge)
{
	float Alpha = FMath::RandBool() ? 1.0f : 0.0f;
    return FMath::Lerp(ShouldDodge, DontDodge, Alpha);
}


void AShooterCharacter::EnemyDodge(AShooterCharacter* EnemyDodge)
{
	FVector LaunchVelocity = EnemyDodge->GetVelocity() * GetRandomVectorBetween(GetActorRightVector() * -1, GetActorRightVector()) * DodgeSpeed;
    EnemyDodge->LaunchCharacter(LaunchVelocity, false, false);
}


void AShooterCharacter::PullTrigger()
{
	UE_LOG(LogTemp, Warning, TEXT("Fire"));
	FHitResult Hit;
	FVector ShotDirection;
	FVector End;

	bool bSuccess = TraceShot(Hit, ShotDirection, End);

	FVector Select = UKismetMathLibrary::SelectVector(Hit.Location, End, bSuccess);
	FVector SocketLocation = GetMesh()->GetSocketLocation(TEXT("BulletSocket"));
	FRotator LookRotation = UKismetMathLibrary::FindLookAtRotation(SocketLocation, Select);
	FTransform LookFire = UKismetMathLibrary::MakeTransform(SocketLocation, LookRotation);
	
	AActor* HitActor = Hit.GetActor();
	if (IsPlayerControlled() && HitActor)
	{
		UCapsuleComponent* HitCapsule = HitActor->FindComponentByClass<UCapsuleComponent>();
		if (HitCapsule)
		{
			// Try to cast the hit actor to a MyCharacter
			AShooterCharacter* EnemyShooter = Cast<AShooterCharacter>(HitActor);
			if (EnemyShooter)
				{
					bool CanDodge = GetRandomBoolBetween(true, false);
					if (CanDodge)
					{
						EnemyDodge(EnemyShooter);
					}
				}
		}
	}
	
	
	if(AugmentBullets <= 1)
	{
		bAugmentReady = false;
	}
	
	AShooterCharacter* Azrael = Cast<AShooterCharacter>(GetOwner());

	if (CanFire)
	{
		if(bAugmentReady && ClipAmmo > 0 && AugmentBullets > 0)
		{
			AugmentBullets -= 1;
			ClipAmmo = ClipAmmo - 1;
		}

		if (ClipAmmo > 0)
		{
			UGameplayStatics::SpawnEmitterAttached(MuzzleMist, GetMesh(), TEXT("BulletSocket"));
			AProjectile *Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, LookFire);
			Projectile->SetOwner(this);
			PlayAnimMontage(RegularFireAnim);
			UE_LOG(LogTemp, Warning, TEXT("Anim Called"));
			ClipAmmo = ClipAmmo - 1;
			// FireRate = PlayAnimMontage(RegularFireAnim);
		}

		else if (TotalAmmo > 0)
		{
			ReloadTimeValid();
		}
		else
		{
			// Need to implement
			TriggerOutOfAmmoPopUp();
		}

		CanFire = false;
		GetWorldTimerManager().SetTimer(FireHandle, this, &AShooterCharacter::FireRateValid, .35, true);
	}
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
	if (IsDead())
	{
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	}

	return DamageToApply;
}

void AShooterCharacter::ReloadTimeValid()
{
	if (ClipAmmo != MaxClipAmmo && ReloadReady && !bAugmentCharge)
	{
		ReloadReady = false;
		PlayAnimMontage(ReloadAnim);
		UGameplayStatics::SpawnEmitterAttached(ReloadParticles, GetMesh(), TEXT("FX_Gun_Barrel"));
		GetWorldTimerManager().SetTimer(ReloadHandle, this, &AShooterCharacter::ReloadGun, ReloadTime, false);
	}
}

void AShooterCharacter::ReloadGun()
{
	if (ClipAmmo != MaxClipAmmo)
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
	ReloadReady = true;
}

void AShooterCharacter::Charge()
{
	bRightClickIsPressed = true;
}

void AShooterCharacter::ChargeTime()
{
	bRightClickIsPressed = false;

	if (fRightClickTime > 1.5)
	{
		ChargeShot();
	}

	fRightClickTime = 0.f;
}

void AShooterCharacter::ChargeShot()
{
	FHitResult Hit;
	FVector ShotDirection;
	FVector End;

	bool bSuccess = TraceShot(Hit, ShotDirection, End);

	FVector Select = UKismetMathLibrary::SelectVector(Hit.Location, End, bSuccess);
	FVector SocketLocation = GetMesh()->GetSocketLocation(TEXT("BulletSocket"));
	FRotator LookRotation = UKismetMathLibrary::FindLookAtRotation(SocketLocation, Select);
	FTransform LookFire = UKismetMathLibrary::MakeTransform(SocketLocation, LookRotation);

	if (bAugmentReady && ClipAmmo > 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("UlitmateShot"));
			UGameplayStatics::SpawnEmitterAttached(UltimateShotPulse, GetMesh(), TEXT("BulletSocket"));
			AProjectile *UltimateShot = GetWorld()->SpawnActor<AProjectile>(UltimateProjectileClass, LookFire);
			PlayAnimMontage(ChargeFireAnim);
			UltimateShot->SetOwner(this);
			ClipAmmo = ClipAmmo - 3;
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.25);
			GetWorldTimerManager().SetTimer(UltiHandle, this, &AShooterCharacter::ResetTimeDilation, .1, true);
		}

		else if (ClipAmmo > 0)
		{
			UGameplayStatics::SpawnEmitterAttached(ElectrifiedPulse, GetMesh(), TEXT("BulletSocket"));
			AProjectile *ChargeShot = GetWorld()->SpawnActor<AProjectile>(ChargedProjectileClass, LookFire);
			PlayAnimMontage(ChargeFireAnim);
			ChargeShot->SetOwner(this);
			ClipAmmo = ClipAmmo - 2;
		}

		else if (TotalAmmo > 0)
		{
			ReloadTimeValid();
			// It's not a bug, its a feature
		}
		else
		{
			TriggerOutOfAmmoPopUp();
		}
	
	bAugmentReady = false;
}

void AShooterCharacter::ThrowGrenade()
{
	PlayAnimMontage(ThrowAnim);
}

void AShooterCharacter::ShockwavePressed()
{
	if(ShockwaveReady)
	{
		ShockwaveReady = false;
		PlayAnimMontage(ShockwaveAnim);
		ShockwaveDuration = PlayAnimMontage(ShockwaveAnim);
		GetWorldTimerManager().SetTimer(ShockwaveHandle, this, &AShooterCharacter::ShockwaveValid, ShockwaveDuration, true);
	}
}

void AShooterCharacter::ShockwaveValid()
{
	ShockwaveReady = true;
}



void AShooterCharacter::AugmentCharge()
{
	if(ReloadReady && isHoldingOrb && OrbAmmo > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("OrbReady"));
		UE_LOG(LogTemp, Warning, TEXT("Orb Ammo: %d"), OrbAmmo);
		isHoldingOrb = true;
		AShooterCharacter* OwningCharacter = Cast<AShooterCharacter>(this);
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = OwningCharacter;
		AAugmentOrb* AugmentOrbInHand = GetWorld()->SpawnActor<AAugmentOrb>(AugmentOrbClass, GetActorTransform(), SpawnParams);
		if(AugmentOrbInHand)
		{
			AugmentOrbInHand->CurrentOrbState = EOrbstate::Option2;
			UE_LOG(LogTemp, Warning, TEXT("Pls"));
			AugmentOrbInHand->SetOwner(OwningCharacter);
			AugmentOrbInHand->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_r_ability_socket"));
			OrbAmmo -= 1;
			// AugmentOrbInHand->SetVisibility(true); 

		}

		
		
		// bAugmentCharge = true;
		// CanFire = false;
		// PlayAnimMontage(AugmentAnim);
	}
}

void AShooterCharacter::AugmentRelease()
{

	bAugmentCharge = false;
	GetCharacterMovement()->MaxWalkSpeed = 800;
	fAugmentChargeTime = 0.f;
	AugmentBullets = 5;
}
