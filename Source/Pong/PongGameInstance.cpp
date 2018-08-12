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
	APlayerController* pc = GetFirstLocalPlayerController(GetWorld());
	if (IsValid(pc))
	{
		return Cast<APongPlayerController>(pc);
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

	APongPlayerController* PongPlayerController = GetPrimaryPlayerController();
	if (IsValid(PongPlayerController))
	{
		PongPlayerController->ClientTravel(IPAddress, ETravelType::TRAVEL_Absolute);
	}
}

void UPongGameInstance::Host()
{
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	World->ServerTravel("Game/MainLevel?listen");
}

void UPongGameInstance::Quit()
{
	APlayerController* pc = GetPrimaryPlayerController();

	if (IsValid(pc))
	{
		pc->ConsoleCommand("Quit");
	}
}