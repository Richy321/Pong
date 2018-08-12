// Fill out your copyright notice in the Description page of Project Settings.

#include "PongAIController.h"
#include "PongBlueprintFunctionLibrary.h"
#include "PongGameMode.h"
#include "PongPawn.h"

void APongAIController::BeginPlay()
{
	Super::BeginPlay();
}

PRAGMA_DISABLE_OPTIMIZATION
void APongAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	APongGameMode* GameMode = UPongBlueprintFunctionLibrary::GetPongGameMode(this);
	if (!IsValid(GameMode))
		return;

	AActor* Ball = GameMode->GetBall();
	APawn* Pawn = GetPawn();

	if (IsValid(Ball) && IsValid(Pawn))
	{
		FVector PaddleLocation = Pawn->GetActorLocation();
		FVector SpawnLine = GameMode->SpawnLineEnd - GameMode->SpawnLineStart;
		FVector BallLocation = Ball->GetActorLocation();

		float distanceToLine = FMath::Abs(PaddleLocation.Y - SpawnLine.Y);
		float distanceToBall = FMath::Abs(PaddleLocation.Y - BallLocation.Y);
		
		if (distanceToBall > distanceToLine)
		{
			//move pawn back to original spawn position (resting state)
			MoveTowards(OriginalPosition);
		}
		else
		{
			//track ball with pawn
			FVector TargetLocation = BallLocation;
			MoveTowards(TargetLocation);
		}
	}
}

void APongAIController::MoveTowards(const FVector& TargetPosition)
{
	APongPawn* pawn = Cast<APongPawn>(GetPawn());
	if (!IsValid(pawn))
	{
		return;
	}

	FVector PaddleLocation = pawn->GetActorLocation();
	if (!FMath::IsNearlyEqual(PaddleLocation.Z, TargetPosition.Z, pawn->MovementSpeed))
	{
		if (TargetPosition.Z > PaddleLocation.Z)
		{
			pawn->VerticalMovement(1.0f);
		}
		else
		{
			pawn->VerticalMovement(-1.0f);
		}
	}
}
PRAGMA_ENABLE_OPTIMIZATION
void APongAIController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	if (IsValid(InPawn))
	{
		OriginalPosition = InPawn->GetActorLocation();
	}
}