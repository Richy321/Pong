// Fill out your copyright notice in the Description page of Project Settings.

#include "PongAIController.h"
#include "PongBlueprintFunctionLibrary.h"
#include "PongGameMode.h"
#include "PongPawn.h"
#include "PhysicsBall.h"
#include "DrawDebugHelpers.h"


APongAIController::APongAIController() :
	EasyDifficultySettings(2.0f, 3.0f, -1.0f, 1.0f),
	NormalDifficultySettings(1.0f, 2.0f, -0.75f, 0.75f),
	HardDifficultySettings(0.0f, 1.0f, -0.5f, 0.5f)
{
}

void APongAIController::BeginPlay()
{
	Super::BeginPlay();
	APongGameMode* GameMode = UPongBlueprintFunctionLibrary::GetPongGameMode(this);
	if (IsValid(GameMode))
	{
		SpawnLineYPosition = GameMode->SpawnLineStart.Y;
	}

	RandomiseDifficulty();
}

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
		//wait for reaction time to finish before doing anything
		if (CurrentReactionTime > 0.0f)
		{
			CurrentReactionTime -= DeltaTime;
			return;
		}

		FVector BallLocation = Ball->GetActorLocation();
	
		const UProjectileMovementComponent& ProjectileMoveComponent = Ball->GetProjectileMovementComponent();
		FVector BallDirection = ProjectileMoveComponent.Velocity.GetSafeNormal();
		//DrawDebugLine(GetWorld(), BallLocation, BallLocation + BallDirection * 1000.0f, FColor::Green, false, 1.0f);

		FVector PaddleFacing(0.0f, bIsLeft ? 1.0f : -1.0f, 0.0f);

		bool bIsBallMovingTowards = FVector::DotProduct(BallDirection, PaddleFacing) < 0;
		if (!bIsBallMovingTowards)
		{		
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



			//Avoid the AI squishing the ball against the side if it has already passed the front of the paddle
			//Also resets paddle to center upon a goal
			if (bIsLeft && (BallLocation.Y < Pawn->GetActorLocation().Y + Pawn->GetPaddleWidth()) ||
				!bIsLeft && (BallLocation.Y > Pawn->GetActorLocation().Y - Pawn->GetPaddleWidth()))
			{
				//move pawn back to original spawn position
				MoveTowards(OriginalPosition.Z, DeltaTime);
			}
			

			//if we change direction (hit a wall) recalc new target position
			if (!BallDirection.Equals(LastDirection, 0.001f))
			{
				TargetLocation = FMath::LinePlaneIntersection(BallLocation, BallLocation + BallDirection * 1000.0f,
					OriginalPosition, FVector(0.0f, 1.0f, 0.0f)); //+ive Y Plane at original position
				
				LastDirection = BallDirection;

				//DrawDebugSphere(GetWorld(), TargetLocation, 50, 32, FColor::Red, false, 1.0f);
				
				//Modify target buy accuracy based on paddle size
				float AccuracyModifier = Pawn->GetPaddleHeight() * CurrentAccuracy;
				TargetLocation.Z += AccuracyModifier;
			}
			
			MoveTowards(TargetLocation.Z, DeltaTime);
		}
	}
}

void APongAIController::MoveTowards(float TargetZ, float DeltaTime)
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

void APongAIController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	if (IsValid(InPawn))
	{
		OriginalPosition = InPawn->GetActorLocation();
		bIsLeft = OriginalPosition.Y < SpawnLineYPosition;
	}
}

void APongAIController::SetDifficulty(EAIDifficulty NewDifficulty)
{
	CurrentDifficulty = NewDifficulty;
	RandomiseDifficulty();
}

void APongAIController::RandomiseDifficulty()
{
	const FAIDifficultySettings& DifficultSettings = GetCurrentDifficultySettings();
	ReactionUpper = FMath::RandRange(DifficultSettings.ReactionMinimum, DifficultSettings.ReactionMaximum);
	ReactionLower = FMath::RandRange(DifficultSettings.ReactionMinimum, DifficultSettings.ReactionMaximum);
	if (ReactionUpper < ReactionLower)
	{
		Swap(ReactionUpper, ReactionLower);
	}

	AccuracyUpper = FMath::RandRange(DifficultSettings.AccuracyMinimum, DifficultSettings.AccuracyMaximum);
	AccuracyLower = FMath::RandRange(DifficultSettings.AccuracyMinimum, DifficultSettings.AccuracyMaximum);
	if (AccuracyUpper < AccuracyLower)
	{
		Swap(AccuracyUpper, AccuracyLower);
	}
}

const FAIDifficultySettings& APongAIController::GetCurrentDifficultySettings()
{
	switch (CurrentDifficulty)
	{
	case EAIDifficulty::Easy:
		return EasyDifficultySettings;
	case EAIDifficulty::Normal:
		return NormalDifficultySettings;
	case EAIDifficulty::Hard:
		return HardDifficultySettings;
	default:
		return NormalDifficultySettings;
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
