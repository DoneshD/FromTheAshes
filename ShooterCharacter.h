// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ChargedProjectile.h"
#include "TimerManager.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimSequence.h"
#include "InputCoreTypes.h"
#include "Particles/ParticleSystem.h"
#include "Components/SkeletalMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "ShooterCharacter.generated.h"


UCLASS()
class DYSTOPIANSHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Not implemented
	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void TriggerOutOfAmmoPopUp();

	bool TraceShot(FHitResult& Hit,  FVector& ShotDirection, FVector& End);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	//Not implemented
	void HandleDestruction();
	
	//Shoot 
	void PullTrigger();
	void ReloadGun();

	//For firerate
	void FireRateValid();
	void ReloadTimeValid();
	void ShockwaveValid();

	//Charge Mechanic
	void Charge();
	void ChargeTime();
	void ChargeShot();

	void AugmentCharge();
	void AugmentRelease();

	void ThrowGrenade();
	void ShockwavePressed();

	void ResetTimeDilation();

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* ChargeFireAnim;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* RegularFireAnim;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* ThrowAnim;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* ReloadAnim;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* AugmentAnim;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* ShockwaveAnim;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	int MaxTotalAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	int MaxClipAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	int TotalAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	int ClipAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxHealth = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float Health = 100;

	FTimerHandle FireHandle;
	FTimerHandle ReloadHandle;
	FTimerHandle ShockwaveHandle;
	FTimerHandle UltiHandle;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool ShockwaveReady = true;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool CanFire = true;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool ReloadReady = true;

	//ChargeShot
	bool bRightClickIsPressed;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float fRightClickTime;

	//AugmentCharge
	bool bAugmentCharge;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float fAugmentChargeTime;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool bAugmentReady = false;


	



private:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	
	void LookUp(float AxisValue);
	void LookRight(float AxisValue);


	//Classes and Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ShockwaveSpawnPoint;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class AProjectile> ChargedProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class AProjectile> UltimateProjectileClass;


	//Regular Effects	
	UPROPERTY(EditAnywhere, Category = "Regular Effects")
	UParticleSystem* MuzzleMist;
	
	UPROPERTY(EditAnywhere, Category = "Regular Effects")
	UParticleSystem* ReloadParticles;

	//Charged Bullets
	UPROPERTY(EditAnywhere, Category = "Charged Effects")
	UParticleSystem* ElectrifiedPulse;

	UPROPERTY(EditAnywhere, Category = "Charged Effects")
	UParticleSystem* ChargingMist;

	//Augment Effects
	UPROPERTY(EditAnywhere, Category = "Augment Effects")
	UParticleSystem* UlimateShotCharging;

	UPROPERTY(EditAnywhere, Category = "Augment Effects")
	UParticleSystem* UltimateShotPulse;

	UPROPERTY(EditAnywhere, Category = "Augment Effects")
	UParticleSystem* AugmentParticles;

	UPROPERTY(EditAnywhere, Category = "Augment Effects")
	UParticleSystem* AugmentEyes;

	UPROPERTY(EditAnywhere, Category = "Augment Effects")
	UParticleSystem* AugmentChargedParticles;

	UPROPERTY(EditAnywhere, Category = "Shockwave Effects")
	UParticleSystem* ShockwaveParticles;





	UPROPERTY(EditAnywhere, Category = "Combat")
	float MaxRange = 10000.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireRate;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float ReloadTime = 2.0;


	float ShockwaveDuration;

};
