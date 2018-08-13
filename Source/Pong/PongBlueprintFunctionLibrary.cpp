// Fill out your copyright notice in the Description page of Project Settings.

#include "PongBlueprintFunctionLibrary.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/GameModeBase.h"
#include "Engine/GameInstance.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

#include "PongGameInstance.h"
#include "PongHUD.h"
#include "PongLobbyHUD.h"
#include "PongGameState.h"
#include "PongGameMode.h"

UPongGameInstance* UPongBlueprintFunctionLibrary::GetPongGameInstance(const UObject* WorldContextObject)
{
	UGameInstance* instance = UGameplayStatics::GetGameInstance(WorldContextObject);
	if (IsValid(instance))
		return Cast<UPongGameInstance>(instance);

	return nullptr;
}

APongGameState* UPongBlueprintFunctionLibrary::GetPongGameState(const UObject* WorldContextObject)
{
	AGameStateBase* State = UGameplayStatics::GetGameState(WorldContextObject);
	if (State)
		return Cast<APongGameState>(State);

	return nullptr;
}

template<typename T>
static T* UPongBlueprintFunctionLibrary::GetDervivedHUD(const UObject* WorldContextObject)
{
	if (!IsValid(WorldContextObject))
	{
		return nullptr;
	}

	UWorld* const World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	if (!World)
	{
		return nullptr;
	}

	//Primary HUD is on controller 0
	APlayerController* player = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	if (IsValid(player))
	{
		return Cast<T>(player->GetHUD());
	}
	return nullptr;
}

APongHUD* UPongBlueprintFunctionLibrary::GetPongHUD(const UObject* WorldContextObject)
{
	return GetDervivedHUD<APongHUD>(WorldContextObject);
}

APongLobbyHUD* UPongBlueprintFunctionLibrary::GetPongLobbyHUD(const UObject* WorldContextObject)
{
	return GetDervivedHUD<APongLobbyHUD>(WorldContextObject);
}

APongGameMode* UPongBlueprintFunctionLibrary::GetPongGameMode(const UObject* WorldContextObject)
{
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(WorldContextObject);
	if (GameMode)
		return Cast<APongGameMode>(GameMode);

	return nullptr;
}

void UPongBlueprintFunctionLibrary::AddOnScreenDebugMessage(FString Text, int Index, float TimeToDisplay, FLinearColor Colour, bool NewerOnTop, FVector2D TextScale)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(Index, TimeToDisplay, Colour.ToFColor(false), Text, NewerOnTop, TextScale);
	}
}