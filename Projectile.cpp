// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "ShooterCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = ProjectileMesh;

	TrailParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Smoke Trail"));
	TrailParticles->SetupAttachment(RootComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	ProjectileMovementComponent->MaxSpeed = 10000.f;
	ProjectileMovementComponent->InitialSpeed = 10000.f;


}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	StartLocation = GetActorLocation();
	
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AActor* MyOwner = GetOwner();

	if(MyOwner == nullptr){
		Destroy();
		return;
	} 
	
	AController* MyOwnerInstigator = MyOwner->GetInstigatorController();
	AActor* HitActor = Hit.GetActor();
	


	if(OtherActor && OtherActor != this && OtherActor != MyOwner)
	{
		if(HitParticles)
			UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, GetActorLocation(), GetActorRotation());\
			FPointDamageEvent DamageEvent(Damage, Hit, NormalImpulse, nullptr);
			HitActor->TakeDamage(Damage, DamageEvent, MyOwnerInstigator, this);

			AShooterCharacter* Shooter = Cast<AShooterCharacter>(MyOwner);
			AShooterCharacter* Enemy = Cast<AShooterCharacter>(HitActor);
			if(Shooter->bAugmentReady && Enemy)
			{
				Shooter->Health += 10;
			}

	}
	Destroy();
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector DistanceDiff = GetActorLocation() - StartLocation;
	if(DistanceDiff.X > PositiveDestroyDistance || DistanceDiff.Y > PositiveDestroyDistance)
	{
		Destroy();
	}


	if(DistanceDiff.X < NegativeDestroyDistance || DistanceDiff.Y < NegativeDestroyDistance)
	{
		Destroy();
	}


}

