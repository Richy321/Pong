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
	AGameStateBase* state = UGameplayStatics::GetGameState(WorldContextObject);
	if (state)
		return Cast<APongGameState>(state);

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
	AGameModeBase* gameMode = UGameplayStatics::GetGameMode(WorldContextObject);
	if (gameMode)
		return Cast<APongGameMode>(gameMode);

	return nullptr;
}

void UPongBlueprintFunctionLibrary::AddOnScreenDebugMessage(FString text, int index, float timeToDisplay, FLinearColor colour, bool newerOnTop, FVector2D textScale)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(index, timeToDisplay, colour.ToFColor(false), text, newerOnTop, textScale);
	}
}