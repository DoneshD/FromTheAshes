// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"


void AShooterAIController::BeginPlay()
{
    
    Super::BeginPlay();

    if(AIBehavior){
        RunBehaviorTree(AIBehavior);
        UE_LOG(LogTemp, Warning, TEXT("Patrol: %s"), *PatrolLocation.ToString());

        APawn *PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
        GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
        GetBlackboardComponent()->SetValueAsVector(TEXT("PatrolLocation"), PatrolLocation);
    }

}

void AShooterAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
