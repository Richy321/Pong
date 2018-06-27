// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SidesEnum.h"
#include "PongHUD.generated.h"

/**
 * 
 */
UCLASS()
class PONG_API APongHUD : public AHUD
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintImplementableEvent, Category = "Apes Game|Events")
	void SetScore(ESides side, int value);
};
