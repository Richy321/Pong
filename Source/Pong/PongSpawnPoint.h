// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SidesEnum.h"
#include "GameTypesEnum.h"
#include "GameFramework/PlayerStart.h"

#include "PongSpawnPoint.generated.h"
/**
 * 
 */

USTRUCT()
struct PONG_API FPongSpawnPoint
{
	GENERATED_BODY()
public:
	FPongSpawnPoint() {}
	FPongSpawnPoint(APlayerStart* PlayerStart, EGameType GameType, ESides Side) : PlayerStart(PlayerStart), GameType(GameType), Side(Side)
	{
	}

	EGameType GameType;
	ESides Side;
	bool bInUse = false;
	APlayerStart* PlayerStart;
};