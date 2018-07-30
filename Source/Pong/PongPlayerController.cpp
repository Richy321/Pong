// Fill out your copyright notice in the Description page of Project Settings.

#include "PongPlayerController.h"
#include "PongBlueprintFunctionLibrary.h"
#include "PongGameState.h"
#include "Camera/CameraActor.h"
#include "PongPawn.h"

APongPlayerController::APongPlayerController()
{
	bAutoManageActiveCameraTarget = false;
}

void APongPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("VerticalMovement", this, &APongPlayerController::VerticalMovement);
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
		//Not on the server, request the server to make the movement
		ServerVerticalMovement(MovementDelta);
	}
}
