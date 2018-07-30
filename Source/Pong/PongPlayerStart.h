// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "SidesEnum.h"
#include "GameTypesEnum.h"

#include "PongPlayerStart.generated.h"
/**
 * 
 */
UCLASS()
class PONG_API APongPlayerStart : public APlayerStart
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EGameType GameType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ESides Side;
	UPROPERTY(BlueprintReadOnly)
	bool bInUse = false;	
};
