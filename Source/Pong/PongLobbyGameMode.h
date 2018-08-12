// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PongLobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PONG_API APongLobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	void BeginPlay() override;

};
