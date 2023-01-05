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

	//Charge Mechanic
	void Charge();
	void ChargeTime();
	void ChargeShot();

	void ThrowGrenade();


	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* ChargeFireAnim;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* RegularFireAnim;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* ThrowAnim;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* ReloadAnim;


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
	bool CanFire = true;

	FTimerHandle ReloadHandle;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool ReloadReady = true;

	//Charging 
	bool bRightClickIsPressed;
	float fRightClickTime;



private:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	
	void LookUp(float AxisValue);
	void LookRight(float AxisValue);


	//Classes and Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class AProjectile> ChargedProjectileClass;


	//Regular Effects	
	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* MuzzleMist;

	//Charged Bullets
	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* ElectrifiedPulse;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* ChargingMist;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* ReloadParticles;



	UPROPERTY(EditAnywhere, Category = "Combat")
	float MaxRange = 10000.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireRate = 0.5;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float ReloadTime = 2.0;

};
