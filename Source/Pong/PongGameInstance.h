// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h" 
#include "PongGameInstance.generated.h"

class APongPlayerController;
/**
 * 
 */
UCLASS()
class PONG_API UPongGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:

	UPongGameInstance(const FObjectInitializer& ObjectInitializer);
	
	APongPlayerController* GetPrimaryPlayerController();
};
