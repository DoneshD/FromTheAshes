// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "OrbState.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum EOrbState
{
	Ammo	UMETA(DisplayName = "Ammo"),
	InHand	UMETA(DisplayName = "InHand"),
	Consumed	UMETA(DisplayName = "Consumed")
};
