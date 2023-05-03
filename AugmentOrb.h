// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OrbState.h"
#include "Components/SphereComponent.h"
#include "AugmentOrb.generated.h"

UCLASS()
class DYSTOPIANSHOOTER_API AAugmentOrb : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAugmentOrb();
	// AAugmentOrb(EOrbState OrbState);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* OrbMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* SphereComponent;

	float OverlapRadius = 150;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	EOrbState OrbState;
private:

};
