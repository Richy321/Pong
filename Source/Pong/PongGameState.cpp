// Fill out your copyright notice in the Description page of Project Settings.

#include "PongGameState.h"
#include "PongBlueprintFunctionLibrary.h"
#include "PongHUD.h"
#include "PongPlayerController.h"
#include "PongGameInstance.h"
#include "GameScore.h"
#include "PongGameMode.h"

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
		UpdateScoreUI();
	}
}

void APongGameState::BeginPlay()
{
	UPongGameInstance* PongGameInstance = UPongBlueprintFunctionLibrary::GetPongGameInstance(GetWorld());
	if (IsValid(PongGameInstance))
	{
		PongGameInstance->SetAppState(EAppState::Game);
	}
	Super::BeginPlay();
}

void APongGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APongGameState, MainCamera);
	DOREPLIFETIME(APongGameState, Score);
	DOREPLIFETIME(APongGameState, StartingCountdownRemaining);
}

void APongGameState::UpdateScoreUI()
{
	APongHUD* HUD = UPongBlueprintFunctionLibrary::GetPongHUD(this);
	if (IsValid(HUD))
	{
		HUD->SetScore(Score);
	}
}

void APongGameState::UpdateStartingCountdownUI()
{
	APongHUD* HUD = UPongBlueprintFunctionLibrary::GetPongHUD(this);
	if (IsValid(HUD))
	{
		HUD->UpdateStartingCountdown(StartingCountdownRemaining);
	}
}

int APongGameState::GetScore(ESides side)
{
	if (side == ESides::Left)
		return Score.ScoreLeft;
	if (side == ESides::Right)
		return Score.ScoreRight;

	return 0;
}

void APongGameState::GameFinished()
{
	//EndMatch();
	State = EGameState::Finished;
	BroadcastGameFinished(Score);
}

void APongGameState::BroadcastGameFinished_Implementation(FGameScore Result)
{
	//show UI
	APongHUD* PongHUD = UPongBlueprintFunctionLibrary::GetPongHUD(GetWorld());
	if (IsValid(PongHUD))
	{
		PongHUD->HideGameUI();
		PongHUD->ShowGameFinishedUI(Result);
	}
}

void APongGameState::GameStarting(float CountdownTime)
{
	StartingCountdownRemaining = CountdownTime;
	State = EGameState::Starting;
	BroadcastGameStarting();
}

void APongGameState::BroadcastGameStarting_Implementation()
{
	//show UI
	APongHUD* PongHUD = UPongBlueprintFunctionLibrary::GetPongHUD(GetWorld());
	if (IsValid(PongHUD))
	{
		PongHUD->ShowGameStartingUI();
	}
}

void APongGameState::GameStarted()
{
	StartingCountdownRemaining = 1.0f;
	State = EGameState::InGame;
	BroadcastGameStarted();
}

void APongGameState::BroadcastGameStarted_Implementation()
{
	//show UI
	APongHUD* PongHUD = UPongBlueprintFunctionLibrary::GetPongHUD(GetWorld());
	if (IsValid(PongHUD))
	{
		PongHUD->ShowGameUI();
	}
}


void APongGameState::Pause()
{
	State = EGameState::Paused;
}

void APongGameState::UnPause()
{
	State = EGameState::InGame;
}

void APongGameState::ResetScore()
{
	//Only let the server reset the score
	if (HasAuthority())
	{
		Score.ScoreLeft = 0;
		Score.ScoreRight = 0;
	}
	UpdateScoreUI();
}