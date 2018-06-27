// Fill out your copyright notice in the Description page of Project Settings.

#include "PongGameMode.h"
#include "Engine/World.h"
#include "Camera/CameraActor.h"
#include "Classes/Kismet/GameplayStatics.h"
#include "Classes/GameFramework/PlayerStart.h"
#include "Runtime/Engine/Public/EngineUtils.h"

#include "PongBlueprintFunctionLibrary.h"
#include "PongGameState.h"
#include "PongGameInstance.h"
#include "PongPlayerController.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PhysicsBall.h"

APongGameMode::APongGameMode()
{

}

AActor* APongGameMode::SpawnBall(ESides spawnSide)
{
	if (IsValid(BallClass))
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

		ball = Cast<APhysicsBall>(GetWorld()->SpawnActor(BallClass, &line));
		UActorComponent* actorComponent = ball->GetComponentByClass(UProjectileMovementComponent::StaticClass());
		if (IsValid(actorComponent))
		{
			UProjectileMovementComponent* projectileComponent = Cast<UProjectileMovementComponent>(actorComponent);
			
			projectileComponent->Velocity = FVector(0.0f, projectileComponent->InitialSpeed * (direction == 0 ? -1.0 : 1.0f), 0.0f);
		}
	}

	//log
	return nullptr;
}

//void APongGameModeBase::SpawnPlayer(int spawnIndex)
//{
//	if (IsValid(PlayerPawnClass))
//	{
//		if (PaddleSpawnPoints.IsValidIndex(spawnIndex))
//		{
//			FVector location = PaddleSpawnPoints[spawnIndex].GetLocation();
//			AActor* actor = GetWorld()->SpawnActor(PlayerPawnClass, &BallSpawnPoint);
//
//
//		}
//		else
//		{
//			//log
//		}
//	}
//}

void APongGameMode::SpawnAI()
{
	if (IsValid(AIPawnClass))
	{

	}
	else
	{
		//log
	}
}


void APongGameMode::IncrementScore(ESides side)
{
	APongGameState* pongState = UPongBlueprintFunctionLibrary::GetPongGameState(GetWorld());
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

		SpawnBall(serveSide);
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
	mainCamera = camera;

	//Set main camera as players view 
	UPongGameInstance* gameInstance = UPongBlueprintFunctionLibrary::GetPongGameInstance(GetWorld());
	if (IsValid(gameInstance))
	{
		APongPlayerController* pc = gameInstance->GetPrimaryPlayerController();
		if (IsValid(pc))
		{
			pc->SetViewTarget(camera);
		}
	}
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
	UClass* PawnClass = GetDefaultPawnClassForController(Player);
	APawn* PawnToFit = PawnClass ? PawnClass->GetDefaultObject<APawn>() : nullptr;
	TArray<APlayerStart*> UnOccupiedStartPoints;
	TArray<APlayerStart*> OccupiedStartPoints;

	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		APlayerStart* PlayerStart = *It;
		FVector ActorLocation = PlayerStart->GetActorLocation();
		const FRotator ActorRotation = PlayerStart->GetActorRotation();

		if (!GetWorld()->EncroachingBlockingGeometry(PawnToFit, ActorLocation, ActorRotation))
		{
			UnOccupiedStartPoints.Add(PlayerStart);
		}
		else if (GetWorld()->FindTeleportSpot(PawnToFit, ActorLocation, ActorRotation))
		{
			OccupiedStartPoints.Add(PlayerStart);
		}
	}

	if (UnOccupiedStartPoints.Num() > 0)
	{
		//TODO - handle spawn position properly.
		for (auto startPoint : UnOccupiedStartPoints)
		{
			const FName LeftPlayerStartTag = FName("Left");

			if (startPoint->PlayerStartTag == LeftPlayerStartTag)
			{
				return startPoint;
			}
		}

		return UnOccupiedStartPoints[FMath::RandRange(0, UnOccupiedStartPoints.Num() - 1)];
	}
	else if (OccupiedStartPoints.Num() > 0)
	{
		return OccupiedStartPoints[FMath::RandRange(0, OccupiedStartPoints.Num() - 1)];
	}
	
	return nullptr;
}
PRAGMA_ENABLE_OPTIMIZATION
