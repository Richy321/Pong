// Fill out your copyright notice in the Description page of Project Settings.

#include "PongGameState.h"
#include "PongBlueprintFunctionLibrary.h"
#include "PongHUD.h"
#include "PongPlayerController.h"
#include "PongGameInstance.h"
#include "GameScore.h"

#include "UnrealNetwork.h"

APongGameState::APongGameState()
{
}

void APongGameState::IncrementScore(ESides Side)
{
	if (Side == ESides::Left)
	{
		Score.ScoreLeft++;
	}
	else if (Side == ESides::Right)
	{
		Score.ScoreRight++;
	}

	APongHUD* hud = UPongBlueprintFunctionLibrary::GetPongHUD(this);
	if (Side != ESides::None)
	{
		UpdateScore();
	}
}

void APongGameState::BeginPlay()
{
}

void APongGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APongGameState, mainCamera);
	DOREPLIFETIME(APongGameState, Score);
}

void APongGameState::UpdateScore()
{
	APongHUD* hud = UPongBlueprintFunctionLibrary::GetPongHUD(this);
	hud->SetScore(Score);
}

int APongGameState::GetScore(ESides side)
{
	if (side == ESides::Left)
		return Score.ScoreLeft;
	if (side == ESides::Right)
		return Score.ScoreRight;

	return 0;
}

void APongGameState::BroadcastGameFinished_Implementation(FGameScore Result)
{
	//show UI
	APongHUD* PongHUD = UPongBlueprintFunctionLibrary::GetPongHUD(GetWorld());
	if (IsValid(PongHUD))
	{
		PongHUD->ShowGameFinishedUI(Result);
	}

	//disable input
	UPongGameInstance* PongGameInstance = UPongBlueprintFunctionLibrary::GetPongGameInstance(GetWorld());
	APongPlayerController* PongPlayerController = PongGameInstance->GetPrimaryPlayerController();
	PongPlayerController->DisableInput(PongPlayerController);
}


void APongGameState::GameFinished()
{
	//EndMatch();
	state = EGameState::Finished;
	BroadcastGameFinished(Score);
}


void APongGameState::Pause()
{
	state = EGameState::Paused;
}

void APongGameState::UnPause()
{
	state = EGameState::InGame;
}