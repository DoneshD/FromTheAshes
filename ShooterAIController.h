// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ShooterAIController.generated.h"

/**
 * 
 */
UCLASS()
class DYSTOPIANSHOOTER_API AShooterAIController : public AAIController
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehavior; 

	FVector PatrolLocation = FVector(7715.903809, -2155.557636, 0);
	
};
