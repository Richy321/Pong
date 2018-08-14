// Fill out your copyright notice in the Description page of Project Settings.

#include "PongPlayerController.h"
#include "PongBlueprintFunctionLibrary.h"
#include "PongGameState.h"
#include "PongGameMode.h"
#include "PongPawn.h"
#include "PongHUD.h"
#include "UI/Menus/MenuScreenWidget.h"
#include "PongGameInstance.h"
#include "PongLobbyHUD.h"

#include "Camera/CameraActor.h"

APongPlayerController::APongPlayerController()
{
	bAutoManageActiveCameraTarget = false;
}

void APongPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("VerticalMovement", this, &APongPlayerController::HandleMovement).bExecuteWhenPaused = true;
	InputComponent->BindAction("Options", IE_Pressed, this, &APongPlayerController::TogglePause).bExecuteWhenPaused = true;
	InputComponent->BindAction("Select", IE_Pressed, this, &APongPlayerController::HandleSelect).bExecuteWhenPaused = true;
	InputComponent->BindAction("Back", IE_Pressed, this, &APongPlayerController::HandleBack).bExecuteWhenPaused = true;
}

void APongPlayerController::ServerVerticalMovement_Implementation(float DeltaMovement)
{
	BroadcastVerticalMovement(DeltaMovement);
}

bool APongPlayerController::ServerVerticalMovement_Validate(float DeltaMovement)
{
	return true;
}

void APongPlayerController::BroadcastVerticalMovement_Implementation(float MovementDelta)
{
	APawn* Pawn = GetPawn();
	if (IsValid(Pawn))
	{
		APongPawn* PongPawn = Cast<APongPawn>(Pawn);
		PongPawn->VerticalMovement(MovementDelta);
	}
}

void APongPlayerController::VerticalMovement(float MovementDelta)
{
	if (Role == ENetRole::ROLE_Authority)
	{
		//On the server directly, make the movement
		APawn* Pawn = GetPawn();
		if (IsValid(Pawn))
		{
			APongPawn* PongPawn = Cast<APongPawn>(Pawn);
			if (IsValid(PongPawn))
			{
				PongPawn->VerticalMovement(MovementDelta);
			}
		}
	}
	else
	{
		//Not on the server, request the server to make the movement (and replicate it back to the client)
		ServerVerticalMovement(MovementDelta);
	}
}

//We use this player controller in both the Lobby and InGame so this function routes input to the correct place
void APongPlayerController::HandleMovement(float DeltaMovement)
{
	if (FMath::IsNearlyEqual(DeltaMovement, 0.0f))
	{
		return;
	}

	//check if we are in game or menus
	UPongGameInstance* PongGameInstance = UPongBlueprintFunctionLibrary::GetPongGameInstance(GetWorld());
	switch (PongGameInstance->GetAppState())
	{
	case EAppState::Lobby:
		{
			APongLobbyHUD* PongLobbyHUD = UPongBlueprintFunctionLibrary::GetPongLobbyHUD(GetWorld());
			PongLobbyHUD->MenuManager->HandleMovement(DeltaMovement);
		}
		break;
	case EAppState::Game:
		{
			APongGameState* PongGameState = UPongBlueprintFunctionLibrary::GetPongGameState(GetWorld());
			if (IsValid(PongGameState))
			{
				if (PongGameState->GetState() == EGameState::InGame ||
					PongGameState->GetState() == EGameState::Waiting ||
					PongGameState->GetState() == EGameState::Starting)
				{
					VerticalMovement(DeltaMovement);
				}
				else
				{
					APongHUD* pongHUD = UPongBlueprintFunctionLibrary::GetPongHUD(GetWorld());
					pongHUD->MenuManager->HandleMovement(DeltaMovement);
				}
			}
		}
		break;
	}

}

void APongPlayerController::HandleSelect()
{
	//check if we are in game or menus
	UPongGameInstance* PongGameInstance = UPongBlueprintFunctionLibrary::GetPongGameInstance(GetWorld());
	switch (PongGameInstance->GetAppState())
	{
		case EAppState::Lobby:
		{
			APongLobbyHUD* PongLobbyHUD = UPongBlueprintFunctionLibrary::GetPongLobbyHUD(GetWorld());
			PongLobbyHUD->MenuManager->HandleSelect();
		}
		break;
		case EAppState::Game:
		{
			APongGameState* PongGameState = UPongBlueprintFunctionLibrary::GetPongGameState(GetWorld());
			if (IsValid(PongGameState) && PongGameState->GetState() != EGameState::InGame)
			{
				APongHUD* PongHUD = UPongBlueprintFunctionLibrary::GetPongHUD(GetWorld());
				PongHUD->MenuManager->HandleSelect();
			}
		}
		break;
	}
}

void APongPlayerController::HandleBack()
{	
	//check if we are in game or menus
	UPongGameInstance* PongGameInstance = UPongBlueprintFunctionLibrary::GetPongGameInstance(GetWorld());
	switch (PongGameInstance->GetAppState())
	{
		case EAppState::Lobby:
		{
			APongLobbyHUD* PongLobbyHUD = UPongBlueprintFunctionLibrary::GetPongLobbyHUD(GetWorld());
			if (IsValid(PongLobbyHUD))
			{
				PongLobbyHUD->MenuManager->HandleBack();
			}
		}
		break;
		case EAppState::Game:
		{
			APongGameState* PongGameState = UPongBlueprintFunctionLibrary::GetPongGameState(GetWorld());
			if (IsValid(PongGameState) && PongGameState->GetState() != EGameState::InGame)
			{
				APongHUD* PongHUD = UPongBlueprintFunctionLibrary::GetPongHUD(GetWorld());
				PongHUD->MenuManager->HandleBack();
			}
		}
		break;
	}
}

void APongPlayerController::TogglePause()
{
	UPongGameInstance* PongGameInstance = UPongBlueprintFunctionLibrary::GetPongGameInstance(GetWorld());
	if (PongGameInstance->GetAppState() != EAppState::Game)
		return;

	APongGameState* State = UPongBlueprintFunctionLibrary::GetPongGameState(this);
	APongHUD* PongHUD = UPongBlueprintFunctionLibrary::GetPongHUD(this);

	switch (State->GetState())
	{
	case EGameState::InGame:
	case EGameState::Waiting:
	case EGameState::Starting:
		Pause();
		State->Pause();
		UPongBlueprintFunctionLibrary::AddOnScreenDebugMessage("Paused Successfully");
		PongHUD->MenuManager->GoToScreen(EMenuScreens::PauseMenu);
		break;
	case EGameState::Paused:
		Pause();
		State->UnPause();
		UPongBlueprintFunctionLibrary::AddOnScreenDebugMessage("UnPaused Successfully");
		PongHUD->MenuManager->GoToScreen(EMenuScreens::None);
		break;
	default:
		break;
	}
}

void APongPlayerController::RequestRematch()
{
	APongGameState* State = UPongBlueprintFunctionLibrary::GetPongGameState(this);
	if (State->GetState() == EGameState::Finished)
	{
		if (Role == ENetRole::ROLE_Authority)
		{
			APongGameMode* PongGameMode = UPongBlueprintFunctionLibrary::GetPongGameMode(this);
			if (IsValid(PongGameMode))
			{
				PongGameMode->RestartGame();
			}
		}
		else
		{
			//send rematch request to server
			ServerRequestRematch();
		}
	}
}

void APongPlayerController::ServerRequestRematch_Implementation()
{
	RequestRematch();
}

bool APongPlayerController::ServerRequestRematch_Validate()
{
	//Only valid if the game has ended. No restarting half way through
	APongGameState* State = UPongBlueprintFunctionLibrary::GetPongGameState(this);
	return State->GetState() == EGameState::Finished;
}
