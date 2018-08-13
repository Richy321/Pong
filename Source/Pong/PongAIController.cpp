// Fill out your copyright notice in the Description page of Project Settings.

#include "PongAIController.h"
#include "PongBlueprintFunctionLibrary.h"
#include "PongGameMode.h"
#include "PongPawn.h"
#include "PhysicsBall.h"

void APongAIController::BeginPlay()
{
	Super::BeginPlay();
	RandomiseDifficulty();
}

PRAGMA_DISABLE_OPTIMIZATION
void APongAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	APongGameMode* GameMode = UPongBlueprintFunctionLibrary::GetPongGameMode(this);
	if (!IsValid(GameMode))
		return;

	APhysicsBall* Ball = GameMode->GetBall();
	APongPawn* Pawn = Cast<APongPawn>(GetPawn());

	if (IsValid(Ball) && IsValid(Pawn))
	{
		FVector PaddleLocation = Pawn->GetActorLocation();
		FVector SpawnLine = GameMode->SpawnLineEnd - GameMode->SpawnLineStart;
		FVector BallLocation = Ball->GetActorLocation();

		float DistanceToLine = FMath::Abs(PaddleLocation.Y - SpawnLine.Y);
		float DistanceToBall = FMath::Abs(PaddleLocation.Y - BallLocation.Y);
		
		if (DistanceToBall > DistanceToLine)
		{
			//move pawn back to original spawn position (resting state)
			MoveTowards(OriginalPosition.Z);
			
			//Reset Reaction and Accuracy
			CurrentReactionTime = -1;
			CurrentAccuracy = -1;
		}
		else
		{
			//get a new reaction for this hit
			if (CurrentReactionTime == -1)
			{
				CurrentReactionTime = GetReaction();
			}

			//get a new accuracy for this hit
			if (CurrentAccuracy == -1)
			{
				CurrentAccuracy = GetAccuracy();
			}

			//track ball with pawn
			//FVector TargetLocation = BallLocation;

			const UProjectileMovementComponent& ProjectileMoveComponent = Ball->GetProjectileMovementComponent();
			FVector TargetLocation = FMath::LinePlaneIntersection(BallLocation, ProjectileMoveComponent.Velocity.GetSafeNormal() * 1000.0f, 
				OriginalPosition, FVector(0.0f, 1.0f, 0.0f)); //+ive Y Plane at original position

			float AccuracyModifier = Pawn->GetPaddleHeight() * CurrentAccuracy;
			float TargetZ = TargetLocation.Z + AccuracyModifier;


			FString TargetLocationMessage = "TargetZ:" + FString::SanitizeFloat(TargetZ);
			FString AccuracyModMessage = "AccuracyMod:" + FString::SanitizeFloat(AccuracyModifier);

			UPongBlueprintFunctionLibrary::AddOnScreenDebugMessage(TargetLocationMessage);
			UPongBlueprintFunctionLibrary::AddOnScreenDebugMessage(AccuracyModMessage);
			MoveTowards(TargetZ);
		}
	}
}

void APongAIController::MoveTowards(float TargetZ)
{
	APongPawn* Pawn = Cast<APongPawn>(GetPawn());
	if (!IsValid(Pawn))
	{
		return;
	}

	FVector PaddleLocation = Pawn->GetActorLocation();

	if (!FMath::IsNearlyEqual(PaddleLocation.Z, TargetZ))
	{
		float ZDistanceToTarget = (TargetZ - PaddleLocation.Z);
		if (ZDistanceToTarget > 0.0f)
		{
			Pawn->VerticalMovement(1.0f);
		}
		else
		{
			Pawn->VerticalMovement(-1.0f);
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

void APongAIController::RandomiseDifficulty()
{
	ReactionUpper = FMath::RandRange(0.0f, 1.0f);
	ReactionLower = FMath::RandRange(0.0f, 1.0f);
	if (ReactionUpper < ReactionLower)
	{
		Swap(ReactionUpper, ReactionLower);
	}

	AccuracyUpper = FMath::RandRange(-0.5f, 0.5f);
	AccuracyLower = FMath::RandRange(-0.5f, 0.5f);
	if (AccuracyUpper < AccuracyLower)
	{
		Swap(AccuracyUpper, AccuracyLower);
	}
}

float APongAIController::GetReaction()
{
	return FMath::RandRange(ReactionLower, ReactionUpper);
}

float APongAIController::GetAccuracy()
{
	return FMath::RandRange(AccuracyLower, AccuracyUpper);
}
