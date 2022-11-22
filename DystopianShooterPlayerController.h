// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DystopianShooterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DYSTOPIANSHOOTER_API ADystopianShooterPlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> HUDclass;

	UPROPERTY(EditAnywhere)
	UUserWidget* HUD;
};
