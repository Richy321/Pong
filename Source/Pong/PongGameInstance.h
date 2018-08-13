// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h" 
#include "PongGameInstance.generated.h"

class APongPlayerController;

UENUM(BlueprintType)
enum class EAppState : uint8
{
	Loading,
	Lobby,
	Game
};


/**
 * 
 */
UCLASS()
class PONG_API UPongGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:

	UPongGameInstance(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION(BlueprintPure)
	APongPlayerController* GetPrimaryPlayerController();

	UFUNCTION(BlueprintCallable)
	void Host();

	UFUNCTION(BlueprintCallable)
	void Join(const FString& IPAddress);
	
	UFUNCTION(BlueprintCallable)
	void LoadLobby();

	UFUNCTION(BlueprintCallable)
	void Quit();

	UFUNCTION(BlueprintCallable)
	EAppState GetAppState() { return AppState; }

	UFUNCTION(BlueprintCallable)
	void SetAppState(EAppState NewState) { AppState = NewState; }

private:
	EAppState AppState = EAppState::Loading;
};
