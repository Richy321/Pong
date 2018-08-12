// Fill out your copyright notice in the Description page of Project Settings.

#include "PongPlayerController.h"
#include "PongBlueprintFunctionLibrary.h"
#include "PongGameState.h"
#include "PongPawn.h"
#include "PongHUD.h"

#include "Camera/CameraActor.h"

APongPlayerController::APongPlayerController()
{
	bAutoManageActiveCameraTarget = false;
}

void APongPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("VerticalMovement", this, &APongPlayerController::VerticalMovement);
	InputComponent->BindAction("Options", IE_Pressed, this, &APongPlayerController::TogglePause).bExecuteWhenPaused = true;
}
void APongPlayerController::BeginPlay()
{
	//Set main camera as players view 
	
	APongGameState* gameState = UPongBlueprintFunctionLibrary::GetPongGameState(this);
	if (gameState)
	{
		SetViewTarget(gameState->mainCamera);
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
	APawn* pawn = GetPawn();
	if (IsValid(pawn))
	{
		APongPawn* pongPawn = Cast<APongPawn>(pawn);
		pongPawn->VerticalMovement(MovementDelta);
	}
}

void APongPlayerController::VerticalMovement(float MovementDelta)
{
	if (Role == ENetRole::ROLE_Authority)
	{
		//On the server directly, make the movement
		APawn* pawn = GetPawn();
		if (IsValid(pawn))
		{
			APongPawn* pongPawn = Cast<APongPawn>(pawn);
			if (IsValid(pongPawn))
			{
				pongPawn->VerticalMovement(MovementDelta);
			}
		}
	}
	else
	{
		//Not on the server, request the server to make the movement (and replicate it back to the client)
		ServerVerticalMovement(MovementDelta);
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