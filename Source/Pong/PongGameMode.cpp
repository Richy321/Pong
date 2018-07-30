// Fill out your copyright notice in the Description page of Project Settings.

#include "PongGameMode.h"
#include "Engine/World.h"
#include "Camera/CameraActor.h"
#include "Classes/Kismet/GameplayStatics.h"
#include "Classes/GameFramework/PlayerStart.h"
#include "Runtime/AIModule/Classes/AIController.h"

#include "Runtime/Engine/Public/EngineUtils.h"

#include "PongBlueprintFunctionLibrary.h"
#include "PongGameState.h"
#include "PongGameInstance.h"
#include "PongPlayerController.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PhysicsBall.h"



#include "Runtime/AIModule/Classes/Blueprint/AIBlueprintHelperLibrary.h"
#include "PongSpawnPoint.h"
#include "PongPlayerStart.h"
#include "PongAIController.h"

APongGameMode::APongGameMode()
{

}

void APongGameMode::ResetBall(AActor* ball, ESides spawnSide)
{
	//pick random point along line
	FVector spawnPoint;

	float randF = FMath::RandRange(0.0f, 1.0f);
	FVector line = (spawnLineEnd - spawnLineStart);
	line = spawnLineStart + (line * randF);

	float direction = 0.0f;
	switch (spawnSide)
	{
	case ESides::None:
		direction = FMath::RandRange(0, 1);
		break;
	case ESides::Left:
		direction = 0;
		break;
	case ESides::Right:
		direction = 1;
		break;
	}
	ball->SetActorLocation(line);
	UActorComponent* actorComponent = ball->GetComponentByClass(UProjectileMovementComponent::StaticClass());
	if (IsValid(actorComponent))
	{
		UProjectileMovementComponent* projectileComponent = Cast<UProjectileMovementComponent>(actorComponent);
		if (IsValid(projectileComponent))
		{
			projectileComponent->Velocity = FVector(0.0f, projectileComponent->InitialSpeed * (direction == 0 ? -1.0 : 1.0f), 0.0f);
		}
	}
}

AActor* APongGameMode::SpawnBall()
{
	if (IsValid(BallClass))
	{
		if (!IsValid(ball))
		{
			ball = Cast<APhysicsBall>(GetWorld()->SpawnActor(BallClass));
		}
		return ball;
	}

	//log
	return nullptr;
}

void APongGameMode::SpawnAI(ESides side)
{
	if (IsValid(AIPawnClass))
	{
		APlayerStart* start = GetFreePlayerStart(GameType, side);
		if (IsValid(start))
		{
			APawn* pawn = UAIBlueprintHelperLibrary::SpawnAIFromClass(GetWorld(), AIPawnClass, nullptr, start->GetActorLocation(), start->GetActorRotation());

			FVector location = pawn->GetActorLocation();
			if (IsValid(pawn->GetController()))
			{
				APongAIController* AIController = Cast<APongAIController>(pawn->GetController());
				if (IsValid(AIController))
				{
					//AIController->UseBlackboard()
				}
			}

		}
	}
	else
	{
		//log
	}
}

void APongGameMode::IncrementScore(ESides side)
{
	UWorld* world = GetWorld();
	if (!IsValid(world))
	{
		return;
	}
		
	APongGameState* pongState = UPongBlueprintFunctionLibrary::GetPongGameState(world);
	if (IsValid(pongState))
	{
		pongState->IncrementScore(side);
		ESides winningSide;
		if (CheckWinState(winningSide))
		{
			EndMatch();
			//trigger winning state on gamestate
		}
		ESides serveSide = ESides::None;

		if (side == ESides::Left)
		{
			serveSide = ESides::Right;
		}
		else if (side == ESides::Right)
		{
			serveSide = ESides::Left;
		}

		ResetBall(ball, serveSide);
	}
}

bool APongGameMode::CheckWinState(ESides &winningSide)
{
	APongGameState* pongState = UPongBlueprintFunctionLibrary::GetPongGameState(GetWorld());
	if (IsValid(pongState))
	{
		for (auto sides : pongState->GetScoreboard())
		{
			if (sides.Value >= NumGoalsToWin)
			{
				winningSide = sides.Key;
				return true;
			}
		}
	}
	return false;
}

void APongGameMode::Init(ACameraActor* camera, FVector halfwayLineStart, FVector halfwayLineEnd)
{
	if (!IsValid(camera))
	{
		//log
		return;
	}
	
	spawnLineStart = halfwayLineStart;
	spawnLineEnd = halfwayLineEnd;
	APongGameState* pongState = UPongBlueprintFunctionLibrary::GetPongGameState(this);
	pongState->mainCamera = camera;


	//Set main camera as players view 
	//UPongGameInstance* gameInstance = UPongBlueprintFunctionLibrary::GetPongGameInstance(GetWorld());
	//if (IsValid(gameInstance))
	//{
	//	APongPlayerController* pc = gameInstance->GetPrimaryPlayerController();
	//	if (IsValid(pc))
	//	{
	//		pc->SetViewTarget(camera);
	//	}
	//}
}


//void APongGameMode::StartMatch()
//{
//	//SpawnBall();
//}

bool APongGameMode::ReadyToStartMatch_Implementation()
{
	return true;
}

bool APongGameMode::ReadyToEndMatch_Implementation()
{
	return false;
}

PRAGMA_DISABLE_OPTIMIZATION
AActor* APongGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	//TODO - tie into lobby

	//Force Left player as PC and right as AI for time being

	if (Player->IsA(APlayerController::StaticClass()))
	{
		//Try to take the left spot
		APlayerStart* start = GetFreePlayerStart(GameType, ESides::Left);
		if (!start)
		{
			//if not available try the right
			start = GetFreePlayerStart(GameType, ESides::Right);
		}
		return start;
	}
	else if (Player->IsA(AAIController::StaticClass()))
	{
		return GetFreePlayerStart(GameType, ESides::Right);
	}

	return nullptr;
}
PRAGMA_ENABLE_OPTIMIZATION

APlayerStart* APongGameMode::GetFreePlayerStart(EGameType gameType, ESides side)
{
	for (TActorIterator<APongPlayerStart> It(GetWorld()); It; ++It)
	{
		
		if (!It->bInUse && It->GameType == gameType && It->Side == side)
		{
			It->bInUse = true;
			return *It;
		}
	}
	return nullptr;
}

AActor* APongGameMode::GetBall()
{
	return ball;
}

void APongGameMode::StartGame()
{
	//Spawn bots
	if (MultiplayerGameType == EMultiplayerGameType::SinglePlayer)
	{
		SpawnAI(ESides::Right);
	}

	AActor* newBall = SpawnBall();
	ResetBall(newBall, ESides::None);
}