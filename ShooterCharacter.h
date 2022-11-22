// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"
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

	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void TriggerOutOfAmmoPopUp();

	bool TraceShot(FHitResult& Hit,  FVector& ShotDirection, FVector& End);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	//Shoot 
	void PullTrigger();
	void ReloadGun();

	//For firerate
	void FireRatedValid();
	void ReloadTimeValid();


private:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	
	void LookUp(float AxisValue);
	void LookRight(float AxisValue);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere)
	UMaterialParameterCollection* collection;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* MuzzleMist;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* ImpactMist;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float MaxRange = 10000.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireRate = 0.2;

public:
	//Ammo magazines
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int MaxTotalAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int MaxClipAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int TotalAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int ClipAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float ReloadTime = 2.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float MaxHealth = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float Health = 100;

	FTimerHandle FireHandle;
	bool CanFire = true;

	FTimerHandle ReloadHandle;
	bool ReloadReady = true;

};
