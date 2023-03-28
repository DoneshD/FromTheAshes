// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OrbState.generated.h"


UENUM(BlueprintType)
enum EOrbstate
{
	Option1 UMETA(DisplayName = "Dormant"),
	Option2 UMETA(DisplayName = "Charging"),
	Option3 UMETA(DisplayName = "Consumed")
};

