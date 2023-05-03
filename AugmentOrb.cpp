// Fill out your copyright notice in the Description page of Project Settings.


#include "AugmentOrb.h"

// Sets default values
AAugmentOrb::AAugmentOrb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OrbMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Orb Mesh"));
	RootComponent = OrbMesh;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	SphereComponent->SetupAttachment(OrbMesh);

	SphereComponent->SetSphereRadius(OverlapRadius);
	

}

// Called when the game starts or when spawned
void AAugmentOrb::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAugmentOrb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

