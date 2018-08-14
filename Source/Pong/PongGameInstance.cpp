// Fill out your copyright notice in the Description page of Project Settings.

#include "PongGameInstance.h"
#include "PongPlayerController.h"

#include "Engine/Engine.h"

UPongGameInstance::UPongGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

APongPlayerController* UPongGameInstance::GetPrimaryPlayerController()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController(GetWorld());
	if (IsValid(PlayerController))
	{
		return Cast<APongPlayerController>(PlayerController);
	}

	return nullptr;
}

void UPongGameInstance::Join(const FString& IPAddress)
{
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("Joining IP: %s"), *IPAddress));

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = GetFirstLocalPlayerController(GetWorld());
	if (IsValid(PlayerController))
	{
		PlayerController->ClientTravel(IPAddress, ETravelType::TRAVEL_Absolute);
	}
}

void UPongGameInstance::LoadLobby()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController(GetWorld());
	if (IsValid(PlayerController))
	{
		PlayerController->ClientTravel("/Game/LobbyLevel", ETravelType::TRAVEL_Absolute);
		AppState = EAppState::Lobby;
	}
}

void UPongGameInstance::Host()
{
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	World->ServerTravel("/Game/MainLevel?listen");
}

void UPongGameInstance::Quit()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController(GetWorld());
	if (IsValid(PlayerController))
	{
		PlayerController->ConsoleCommand("quit");
	}
}

