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
		SetDifficulty(CurrentDifficulty);
	}
}

void APongAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	APongGameMode* GameMode = UPongBlueprintFunctionLibrary::GetPongGameMode(this);
	if (!IsValid(GameMode))
		return;

	if (bIsShowDifficultySettings)
	{
		DrawDifficultySettings();
	}

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
			CurrentReaction = -1;
			CurrentReactionTime = 0.0f;
			CurrentAccuracy = -1;
		}
		else
		{
			//get a new reaction for this hit
			if (CurrentReaction == -1)
			{
				CurrentReaction = GetNewReaction();
				CurrentReactionTime = CurrentReaction;

				//half reaction time if it's a serve to give AI more of a chance as
				//it's half the distance
				if (Ball->LastHitter == nullptr)
				{
					CurrentReactionTime = 0.5f;
				}
			}

			//get a new accuracy for this hit
			if (CurrentAccuracy == -1)
			{
				CurrentAccuracy = GetNewAccuracy();
			}

			//if the ball changed direction (hit a wall) recalc new target position
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

			//Avoid the AI squishing the ball against the side if it has already passed the front of the paddle
			//Also resets paddle to center upon a goal
			float epsilon = 2.0f;
			FVector PaddleLocation = Pawn->GetActorLocation();
			float PaddleWidth = Pawn->GetPaddleWidth();

			if ((bIsLeft && (BallLocation.Y < PaddleLocation.Y + PaddleWidth + epsilon)) ||
				(!bIsLeft && (BallLocation.Y > PaddleLocation.Y - PaddleWidth + epsilon)))
			{
				//Reset Reaction and Accuracy ready for serve
				CurrentReactionTime = 0.0f;
				CurrentReaction = -1;
				CurrentAccuracy = -1;
				LastDirection = FVector::ZeroVector;
				
				//Set targett to original spawn position
				TargetLocation = OriginalPosition;
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

	float Tolerance = 1.0f;
	if (!FMath::IsNearlyEqual(PaddleLocation.Z, TargetZ, Tolerance))
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
}

void APongAIController::ShowDifficultySettings(bool Show)
{
	bIsShowDifficultySettings = Show;
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

float APongAIController::GetNewReaction()
{
	return FMath::RandRange(GetCurrentDifficultySettings().ReactionMinimum, GetCurrentDifficultySettings().ReactionMaximum);
}

float APongAIController::GetNewAccuracy()
{
	return FMath::RandRange(GetCurrentDifficultySettings().AccuracyMinimum, GetCurrentDifficultySettings().AccuracyMaximum);
}

void APongAIController::DrawDifficultySettings()
{
	const FAIDifficultySettings&  CurrentDifficultySettings = GetCurrentDifficultySettings();
	FString DifficultyLevelText = "DifficultyLevel:" + AIDifficultyEnumToString(CurrentDifficulty);
	UPongBlueprintFunctionLibrary::AddOnScreenDebugMessage(DifficultyLevelText, 0);

	FString ReactionText = FString::Printf(TEXT("Reaction:  %f (%f) (%f-%f)"), CurrentReaction, CurrentReactionTime, CurrentDifficultySettings.ReactionMinimum, CurrentDifficultySettings.ReactionMaximum);
	UPongBlueprintFunctionLibrary::AddOnScreenDebugMessage(ReactionText, 1);

	FString AccuracyText = FString::Printf(TEXT("Accuracy: %f (%f-%f)"), CurrentAccuracy, CurrentDifficultySettings.AccuracyMinimum, CurrentDifficultySettings.AccuracyMaximum);
	UPongBlueprintFunctionLibrary::AddOnScreenDebugMessage(AccuracyText, 2);
}