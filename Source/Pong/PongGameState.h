// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "SidesEnum.h"
#include "Runtime/Engine/Classes/Engine/TriggerVolume.h"
#include "PongGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnScoreIncrementDelegate);

UENUM(BlueprintType)
enum class EGameState : uint8
{
	InGame,
	Paused,
	Finished
};

UCLASS()
class PONG_API APongGameState : public AGameState
{
	GENERATED_BODY()

public:

	APongGameState();

	UPROPERTY(BlueprintReadWrite)
	EGameState state;

	void IncrementScore(ESides side);
	UPROPERTY(BlueprintAssignable)
	FOnScoreIncrementDelegate OnScoreIncrement;

	UFUNCTION(BlueprintCallable)
	const TMap<ESides, int> GetScoreboard() const;

	UPROPERTY(BlueprintReadWrite)
	TArray<ATriggerVolume*> goals;

	void BeginPlay() override;

private:
	TMap<ESides, int> scoreboard;
};
