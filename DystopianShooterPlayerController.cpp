// Fill out your copyright notice in the Description page of Project Settings.


#include "DystopianShooterPlayerController.h"
#include "Blueprint/UserWidget.h"


void ADystopianShooterPlayerController::BeginPlay()
{
    Super::BeginPlay();
    HUD = CreateWidget(this, HUDclass);
    if(HUD){
        HUD->AddToViewport();
    }

}