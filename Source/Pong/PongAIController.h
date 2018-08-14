// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "AIDifficultyEnum.h"
#include "PongAIController.generated.h"


USTRUCT(BlueprintType)
struct FAIDifficultySettings
{
	GENERATED_BODY()
public:
	FAIDifficultySettings() 
	{}

	FAIDifficultySettings(float ReactionMin, float ReactionMax, float AccuracyMin, float AccuracyMax) : 
		ReactionMinimum(ReactionMin), ReactionMaximum(ReactionMax), 
		AccuracyMinimum(AccuracyMin), AccuracyMaximum(AccuracyMax)
	{}

	UPROPERTY(EditAnywhere)
	float ReactionMinimum = 0.0f;
	UPROPERTY(EditAnywhere)
	float ReactionMaximum = 1.0f;

	UPROPERTY(EditAnywhere)
	float AccuracyMinimum = -0.5f;
	UPROPERTY(EditAnywhere)
	float AccuracyMaximum = 0.5f;
};

/**
 * 
 */
UCLASS()
class PONG_API APongAIController : public AAIController
{
	GENERATED_BODY()
public:

	APongAIController();

	void Tick(float DeltaTime) override;
	void BeginPlay() override;

	void SetPawn(APawn* InPawn) override;
	
	void SetDifficulty(EAIDifficulty NewDifficulty);

	UPROPERTY(EditAnywhere)
	FAIDifficultySettings EasyDifficultySettings;
	UPROPERTY(EditAnywhere)
	FAIDifficultySettings NormalDifficultySettings;
	UPROPERTY(EditAnywhere)
	FAIDifficultySettings HardDifficultySettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EAIDifficulty CurrentDifficulty = EAIDifficulty::Normal;
private:
	void MoveTowards(float TargetZ, float DeltaTime);

	void RandomiseDifficulty();
	const FAIDifficultySettings& GetCurrentDifficultySettings();

	FVector OriginalPosition;
	bool bIsLeft;
	
	float SpawnLineYPosition;

	float ReactionUpper;
	float ReactionLower;

	float AccuracyUpper;
	float AccuracyLower;

	float GetReaction();
	float GetAccuracy();

	float CurrentAccuracy = -1.0f;
	float CurrentReactionTime = -1.0f;

	FVector LastDirection;
	FVector TargetLocation;
};
