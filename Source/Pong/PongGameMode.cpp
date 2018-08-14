// Fill out your copyright notice in the Description page of Project Settings.

#include "PongGameMode.h"

#include "Engine/World.h"
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
#include "PongPlayerStart.h"

APongGameMode::APongGameMode()
{

}

void APongGameMode::ResetBall(APhysicsBall* Ball, ESides SpawnSide)
{
	//pick random point along line
	FVector SpawnPoint;
	
	//clear last hitter to indicate serve
	Ball->LastHitter = nullptr;

	//Pick a random point on the line avoiding the end and beginning 25%
	float RandF = FMath::RandRange(0.25f, 0.75f);
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

void APongGameMode::Serve(APhysicsBall* Ball, ESides SpawnSide, float DelayServeTime)
{
	if (DelayServeTime > 0.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(ServeDelayTimerHandle);

		//Delay Serve
		FTimerDelegate DelayStartCallback;
		DelayStartCallback.BindLambda([this, Ball, SpawnSide]
		{
			ResetBall(Ball, SpawnSide);
		});
		GetWorld()->GetTimerManager().SetTimer(ServeDelayTimerHandle, DelayStartCallback, DelayServeTime, false);
	}
	else
	{
		ResetBall(Ball, SpawnSide);
	}
}

APhysicsBall* APongGameMode::SpawnBall()
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
		UPongGameInstance* PongGameInstance = UPongBlueprintFunctionLibrary::GetPongGameInstance(GetWorld());

		APlayerStart* Start = GetFreePlayerStart(PongGameInstance->GameType, Side);
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

			Serve(Ball, ServeSide, ServeDelay);
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

void APongGameMode::Init(FVector HalfwayLineStart, FVector HalfwayLineEnd)
{
	SpawnLineStart = HalfwayLineStart;
	SpawnLineEnd = HalfwayLineEnd;
}

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
	//Force left player as PC and right as AI 
	UPongGameInstance* PongGameInstance = UPongBlueprintFunctionLibrary::GetPongGameInstance(GetWorld());

	if (Player->IsA(APlayerController::StaticClass()))
	{
		//Try to take the left spot
		APlayerStart* Start = GetFreePlayerStart(PongGameInstance->GameType, ESides::Left);
		if (!Start)
		{
			//if not available try the right
			Start = GetFreePlayerStart(PongGameInstance->GameType, ESides::Right);
		}
		return Start;
	}
	else if (Player->IsA(AAIController::StaticClass()))
	{
		return GetFreePlayerStart(PongGameInstance->GameType, ESides::Right);
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

APhysicsBall* APongGameMode::GetBall()
{
	return Ball;
}

void APongGameMode::StartGame()
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}

	UPongGameInstance* PongGameInstance = UPongBlueprintFunctionLibrary::GetPongGameInstance(World);
	//Spawn bots
	if (PongGameInstance->MultiplayerGameType == EMultiplayerGameType::SinglePlayer)
	{
		if (!IsValid(AIController))
		{
			AIController = SpawnAI(ESides::Right);
			AIController->SetDifficulty(PongGameInstance->AIDifficulty);
		}
	}

	APongGameState* PongState = UPongBlueprintFunctionLibrary::GetPongGameState(World);
	if (IsValid(PongState))
	{
		PongState->GameStarting(StartingCountdown);
	}

	GetWorld()->GetTimerManager().ClearTimer(StartingCountdownTimerHandle);

	FTimerDelegate DeactivateRepeatDelayCallback;
	DeactivateRepeatDelayCallback.BindLambda([this]
	{
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
	UPongGameInstance* PongGameInstance = UPongBlueprintFunctionLibrary::GetPongGameInstance(GetWorld());
	switch (PongGameInstance->MultiplayerGameType)
	{
	case EMultiplayerGameType::SinglePlayer:
		return 1;
	case EMultiplayerGameType::Multiplayer:
		switch (PongGameInstance->GameType)
		{
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
				GetWorld()->GetTimerManager().SetTimer(JoinStartDelayTimerHandle, DelayStartCallback, 1.0f, false);
			}
		}
	}
}

void APongGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorld()->GetTimerManager().ClearTimer(StartingCountdownTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(JoinStartDelayTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(ServeDelayTimerHandle);
}