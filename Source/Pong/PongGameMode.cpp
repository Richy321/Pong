// Fill out your copyright notice in the Description page of Project Settings.

#include "PongGameMode.h"

#include "Engine/World.h"
#include "Camera/CameraActor.h"
#include "Classes/Kismet/GameplayStatics.h"
#include "Classes/GameFramework/PlayerStart.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "Runtime/AIModule/Classes/Blueprint/AIBlueprintHelperLibrary.h"
#include "TimerManager.h"

#include "PongBlueprintFunctionLibrary.h"
#include "PongGameState.h"
#include "PongGameInstance.h"
#include "PongHUD.h"
#include "PongPlayerController.h"
#include "PongAIController.h"
#include "PhysicsBall.h"
#include "PongSpawnPoint.h"
#include "PongPlayerStart.h"

APongGameMode::APongGameMode()
{

}

void APongGameMode::ResetBall(AActor* Ball, ESides SpawnSide)
{
	//pick random point along line
	FVector SpawnPoint;

	float RandF = FMath::RandRange(0.0f, 1.0f);
	FVector Line = (SpawnLineEnd - SpawnLineStart);
	Line = SpawnLineStart + (Line * RandF);

	float Direction = 0.0f;
	switch (SpawnSide)
	{
	case ESides::None:
		Direction = FMath::RandRange(0, 1);
		break;
	case ESides::Left:
		Direction = 0;
		break;
	case ESides::Right:
		Direction = 1;
		break;
	}
	Ball->SetActorLocation(Line);
	UActorComponent* ActorComponent = Ball->GetComponentByClass(UProjectileMovementComponent::StaticClass());
	if (IsValid(ActorComponent))
	{
		UProjectileMovementComponent* ProjectileComponent = Cast<UProjectileMovementComponent>(ActorComponent);
		if (IsValid(ProjectileComponent))
		{
			ProjectileComponent->Velocity = FVector(0.0f, ProjectileComponent->InitialSpeed * (Direction == 0 ? -1.0 : 1.0f), 0.0f);
		}
	}
}

AActor* APongGameMode::SpawnBall()
{
	if (IsValid(BallClass))
	{
		if (!IsValid(Ball))
		{
			Ball = Cast<APhysicsBall>(GetWorld()->SpawnActor(BallClass));
		}
		return Ball;
	}

	//log
	return nullptr;
}

APongAIController* APongGameMode::SpawnAI(ESides Side)
{
	if (IsValid(AIPawnClass))
	{
		APlayerStart* Start = GetFreePlayerStart(GameType, Side);
		if (IsValid(Start))
		{
			APawn* Pawn = UAIBlueprintHelperLibrary::SpawnAIFromClass(GetWorld(), AIPawnClass, nullptr, Start->GetActorLocation(), Start->GetActorRotation());

			FVector location = Pawn->GetActorLocation();
			if (IsValid(Pawn->GetController()))
			{
				APongAIController* NewAIController = Cast<APongAIController>(Pawn->GetController());
				if (IsValid(NewAIController))
				{
					return NewAIController;
				}
			}

		}
	}
	UE_LOG(LogTemp, Warning, TEXT("%s - Failed to spawn AI controller"), *FString(__FUNCTION__));

	return nullptr;
}

void APongGameMode::IncrementScore(ESides Side)
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}
		
	APongGameState* PongState = UPongBlueprintFunctionLibrary::GetPongGameState(World);
	if (IsValid(PongState))
	{
		PongState->IncrementScore(Side);
		ESides WinningSide;
		if (CheckWinState(WinningSide))
		{
			//trigger winning state on gamestate
			PongState->GameFinished();
		}
		else
		{
			ESides ServeSide = ESides::None;

			if (Side == ESides::Left)
			{
				ServeSide = ESides::Right;
			}
			else if (Side == ESides::Right)
			{
				ServeSide = ESides::Left;
			}

			ResetBall(Ball, ServeSide);
		}
	}
}

bool APongGameMode::CheckWinState(ESides &WinningSide)
{
	APongGameState* PongState = UPongBlueprintFunctionLibrary::GetPongGameState(GetWorld());
	if (IsValid(PongState))
	{
		if (PongState->GetScore(ESides::Left) >= NumGoalsToWin)
		{
			WinningSide = ESides::Left;
			return true;
		}
		else if (PongState->GetScore(ESides::Right) >= NumGoalsToWin)
		{
			WinningSide = ESides::Right;
			return true;
		}
	}
	return false;
}

void APongGameMode::Init(ACameraActor* Camera, FVector HalfwayLineStart, FVector HalfwayLineEnd)
{
	if (!IsValid(Camera))
	{
		//log
		return;
	}
	
	SpawnLineStart = HalfwayLineStart;
	SpawnLineEnd = HalfwayLineEnd;
	APongGameState* PongState = UPongBlueprintFunctionLibrary::GetPongGameState(this);
	PongState->mainCamera = Camera;
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

APlayerStart* APongGameMode::GetFreePlayerStart(EGameType GameType, ESides Side)
{
	for (TActorIterator<APongPlayerStart> It(GetWorld()); It; ++It)
	{
		
		if (!It->bInUse && It->GameType == GameType && It->Side == Side)
		{
			It->bInUse = true;
			return *It;
		}
	}
	return nullptr;
}

AActor* APongGameMode::GetBall()
{
	return Ball;
}

void APongGameMode::StartGame()
{
	APongGameState* PongState = UPongBlueprintFunctionLibrary::GetPongGameState(GetWorld());
	if (IsValid(PongState))
	{
		PongState->GameStarting(StartingCountdown);
	}

	GetWorld()->GetTimerManager().ClearTimer(StartingCountdownTimerHandle);

	FTimerDelegate DeactivateRepeatDelayCallback;
	DeactivateRepeatDelayCallback.BindLambda([this]
	{
		//Spawn bots
		if (MultiplayerGameType == EMultiplayerGameType::SinglePlayer)
		{
			if (!IsValid(AIController))
			{
				AIController = SpawnAI(ESides::Right);
			}
		}

		SpawnBall();
		ResetBall(Ball, ESides::None);
		APongGameState* PongState = UPongBlueprintFunctionLibrary::GetPongGameState(GetWorld());
		if (IsValid(PongState))
		{
			PongState->GameStarted();
		}
	});

	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(StartingCountdownTimerHandle, DeactivateRepeatDelayCallback, StartingCountdown, false);
}

void APongGameMode::RestartGame()
{
	APongGameState* PongState = UPongBlueprintFunctionLibrary::GetPongGameState(GetWorld());
	if (IsValid(PongState))
	{
		PongState->ResetScore();
	}
	StartGame();
}

void APongGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!HasAuthority())
		return;

	APongGameState* PongState = UPongBlueprintFunctionLibrary::GetPongGameState(GetWorld());
	if (IsValid(PongState))
	{
		if (PongState->GetState() == EGameState::Starting)
		{
			PongState->StartingCountdownRemaining = FMath::Max(1.0f, PongState->StartingCountdownRemaining - DeltaSeconds);
			PongState->UpdateStartingCountdownUI();
		}
	}
}

int APongGameMode::GetRequiredPlayerCount()
{
	switch (MultiplayerGameType)
	{
	case EMultiplayerGameType::SinglePlayer:
		return 1;
	case EMultiplayerGameType::OnlineMultiplayer:
	case EMultiplayerGameType::LocalMultiplayer:
		switch (GameType)
		{
		case EGameType::TwoVsTwo:
			return 4;
		case EGameType::OneVsOne:
		default:
			return 2;
		}
	}
	return 1;
}

void APongGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	APongGameState* PongState = UPongBlueprintFunctionLibrary::GetPongGameState(GetWorld());
	if (IsValid(PongState))
	{
		//Check if all players have joined
		if (PongState->GetState() == EGameState::Waiting)
		{
			if (PongState->PlayerArray.Num() == GetRequiredPlayerCount())
			{
				//Delay before calling StartGame to allow Client to be fully ready. 
				//(hasn't finished initialising fully at this point for some reason)
				FTimerDelegate DelayStartCallback;
				DelayStartCallback.BindLambda([this]
				{
					StartGame();
				});
				FTimerHandle Handle;
				GetWorld()->GetTimerManager().SetTimer(Handle, DelayStartCallback, 1.0f, false);
			}
		}
	}
}