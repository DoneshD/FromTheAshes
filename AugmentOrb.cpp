// Fill out your copyright notice in the Description page of Project Settings.

#include "AugmentOrb.h"
#include "OrbState.h"

// Sets default values
AAugmentOrb::AAugmentOrb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OrbMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Orb Mesh"));
	RootComponent = OrbMesh;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	SphereComponent->SetupAttachment(RootComponent);

	SphereComponent->SetSphereRadius(OverlapRadius);
	// SphereComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	// SphereComponent->AttachToComponent(OrbMesh, FAttachmentTransformRules::KeepRelativeTransform);


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

// void AAugmentOrb::SetVisibility(bool bVisible)
// {
//     SetActorHiddenInGame(!bVisible);
//     SetActorEnableCollision(bVisible);
//     SetActorTickEnabled(bVisible);
// }


