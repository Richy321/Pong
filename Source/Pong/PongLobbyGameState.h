// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PongLobbyGameState.generated.h"

/**
 * 
 */
UCLASS()
class PONG_API APongLobbyGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	void BeginPlay() override;
};
