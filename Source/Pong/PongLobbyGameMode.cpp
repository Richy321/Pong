// Fill out your copyright notice in the Description page of Project Settings.

#include "PongLobbyGameMode.h"
#include "PongGameInstance.h"
#include "PongBlueprintFunctionLibrary.h"

void APongLobbyGameMode::BeginPlay()
{
	UPongGameInstance* PongGameInstance = UPongBlueprintFunctionLibrary::GetPongGameInstance(GetWorld());
	if (IsValid(PongGameInstance))
	{
		PongGameInstance->SetAppState(EAppState::Lobby);
	}
	Super::BeginPlay();
}
