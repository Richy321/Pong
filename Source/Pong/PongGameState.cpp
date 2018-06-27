// Fill out your copyright notice in the Description page of Project Settings.

#include "PongGameState.h"
#include "PongBlueprintFunctionLibrary.h"
#include "PongHUD.h"

APongGameState::APongGameState()
{
	scoreboard.Add(ESides::Left, 0);
	scoreboard.Add(ESides::Right, 0);
}

void APongGameState::IncrementScore(ESides side)
{
	if (scoreboard.Contains(side))
	{
		scoreboard[side]++;
		APongHUD* hud = UPongBlueprintFunctionLibrary::GetPongHUD(this);
		hud->SetScore(side, scoreboard[side]);
		OnScoreIncrement.Broadcast();
	}
	else
	{
		//log
	}
}

const TMap<ESides, int> APongGameState::GetScoreboard() const
{
	return scoreboard;
}

void APongGameState::BeginPlay()
{
	for (ATriggerVolume* volume : goals)
	{

	}
}