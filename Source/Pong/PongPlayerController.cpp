// Fill out your copyright notice in the Description page of Project Settings.

#include "PongPlayerController.h"
#include "PongBlueprintFunctionLibrary.h"
#include "PongGameState.h"
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
void APongPlayerController::BeginPlay()
{
	//Set main camera as players view 
	
	APongGameState* GameState = UPongBlueprintFunctionLibrary::GetPongGameState(this);
	if (GameState)
	{
		SetViewTarget(GameState->mainCamera);
	}
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

//We reuse this player controller in both the Lobby and InGame so input this function reroutes input to the correct place
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
			APongLobbyHUD* pongLobbyHUD = UPongBlueprintFunctionLibrary::GetPongLobbyHUD(GetWorld());
			pongLobbyHUD->MenuManager->HandleMovement(DeltaMovement);
		}
		break;
	case EAppState::Game:
		{
			APongGameState* PongGameState = UPongBlueprintFunctionLibrary::GetPongGameState(GetWorld());
			if (PongGameState->GetState() == EGameState::InGame)
			{
				VerticalMovement(DeltaMovement);
			}
			else
			{
				APongHUD* pongHUD = UPongBlueprintFunctionLibrary::GetPongHUD(GetWorld());
				pongHUD->MenuManager->HandleMovement(DeltaMovement);
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
			APongLobbyHUD* pongLobbyHUD = UPongBlueprintFunctionLibrary::GetPongLobbyHUD(GetWorld());
			pongLobbyHUD->MenuManager->HandleSelect();
		}
		break;
		case EAppState::Game:
		{
			APongGameState* PongGameState = UPongBlueprintFunctionLibrary::GetPongGameState(GetWorld());
			if (PongGameState->GetState() != EGameState::InGame)
			{
				APongHUD* pongHUD = UPongBlueprintFunctionLibrary::GetPongHUD(GetWorld());
				pongHUD->MenuManager->HandleSelect();
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
			APongLobbyHUD* pongLobbyHUD = UPongBlueprintFunctionLibrary::GetPongLobbyHUD(GetWorld());
			pongLobbyHUD->MenuManager->HandleBack();
		}
		break;
		case EAppState::Game:
		{
			APongGameState* PongGameState = UPongBlueprintFunctionLibrary::GetPongGameState(GetWorld());
			if (PongGameState->GetState() != EGameState::InGame)
			{
				APongHUD* pongHUD = UPongBlueprintFunctionLibrary::GetPongHUD(GetWorld());
				pongHUD->MenuManager->HandleBack();
			}
		}
		break;
	}
}

//void APongPlayerController::Pause()
//{
//	APongGameState* state = UPongBlueprintFunctionLibrary::GetPongGameState(this);
//	if (state->GetState() == EGameState::Paused)
//	{
//		return;
//	}
//
//	if (Role == ENetRole::ROLE_Authority)
//	{
//		//On the server directly, pause the game
//		state->GetState()
//
//	}
//	else
//	{
//		//Not on the server, request the server to make the movement (and replicate it back to the client)
//		ServerPause();
//	}
//}
//
//void APongPlayerController::UnPause()
//{
//	APongGameState* state = UPongBlueprintFunctionLibrary::GetPongGameState(this);
//	if (state->GetState() != EGameState::Paused)
//	{
//		return;
//	}
//
//	if (Role == ENetRole::ROLE_Authority)
//	{
//		//On the server directly, pause the game
//
//
//	}
//	else
//	{
//		//Not on the server, request the server to make the movement (and replicate it back to the client)
//		ServerPause();
//	}
//}


//void APongPlayerController::ServerPause_Implementation()
//{
//	//BroadcastPause();
//}
//
//bool APongPlayerController::ServerPause_Validate()
//{
//	return true;
//}
//
//void APongPlayerController::ServerUnPause_Implementation()
//{
//	//BroadcastUnPause();
//}
//
//bool APongPlayerController::ServerUnPause_Validate()
//{
//	return true;
//}

void APongPlayerController::TogglePause()
{
	APongGameState* State = UPongBlueprintFunctionLibrary::GetPongGameState(this);
	APongHUD* PongHUD = UPongBlueprintFunctionLibrary::GetPongHUD(this);

	switch (State->GetState())
	{
	case EGameState::InGame:
		if (SetPause(true))
		{
			State->Pause();
			UPongBlueprintFunctionLibrary::AddOnScreenDebugMessage("Paused Successfully");
		}
		else
		{
			UPongBlueprintFunctionLibrary::AddOnScreenDebugMessage("Pause failed");
		}

		PongHUD->MenuManager->GoToScreen(EMenuScreens::PauseMenu);
		break;
	case EGameState::Paused:
		if (SetPause(false))
		{
			State->UnPause();
			UPongBlueprintFunctionLibrary::AddOnScreenDebugMessage("UnPaused Successfully");
		}
		else
		{
			UPongBlueprintFunctionLibrary::AddOnScreenDebugMessage("UnPause failed");
		}
		PongHUD->MenuManager->GoToScreen(EMenuScreens::None);
		break;
	default:
		break;
	}
}

void APongPlayerController::LeaveGame()
{
	//RPC to quit?
	ClientTravel("Game/MainMenu", ETravelType::TRAVEL_Absolute);

}