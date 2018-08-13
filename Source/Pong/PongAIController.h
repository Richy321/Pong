// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PongAIController.generated.h"

/**
 * 
 */
UCLASS()
class PONG_API APongAIController : public AAIController
{
	GENERATED_BODY()
public:

	void Tick(float DeltaTime) override;
	void BeginPlay() override;

	void SetPawn(APawn* InPawn) override;

	void RandomiseDifficulty();

private:
	void MoveTowards(float TargetZ);
	FVector OriginalPosition;
	
	float ReactionUpper;
	float ReactionLower;

	float AccuracyUpper;
	float AccuracyLower;

	float GetReaction();
	float GetAccuracy();

	float CurrentAccuracy = -1.0f;
	float CurrentReactionTime = -1.0f;
};
