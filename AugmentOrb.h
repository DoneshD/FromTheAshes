// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "OrbState.h"
#include "GameFramework/Actor.h"
#include "AugmentOrb.generated.h"

UCLASS()
class DYSTOPIANSHOOTER_API AAugmentOrb : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAugmentOrb();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* OrbMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* SphereComponent;

	EOrbstate CurrentOrbState;

	// void SetVisibility(bool bVisible);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	float OverlapRadius = 150;

};
