// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Runtime/Engine/Classes/Engine/TriggerVolume.h"

#include "GameScore.h"
#include "SidesEnum.h"
#include "PongGameState.generated.h"

UENUM(BlueprintType)
enum class EGameState : uint8
{
	Waiting,
	Starting,
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

	inline EGameState GetState() { return state; }

	void IncrementScore(ESides side);

	UPROPERTY(BlueprintReadWrite)
	TArray<ATriggerVolume*> goals;

	void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, replicated)
	ACameraActor* mainCamera;

	UFUNCTION()
	void UpdateScoreUI();

	UFUNCTION()
	void UpdateStartingCountdownUI();

	UFUNCTION()
	int GetScore(ESides side);

	UFUNCTION()
	void Pause();
	
	UFUNCTION()
	void UnPause();

	UFUNCTION()
	void ResetScore();

	UFUNCTION()
	void GameFinished();

	UFUNCTION()
	void GameStarting(float CountdownTime);

	UFUNCTION()
	void GameStarted();

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = UpdateStartingCountdownUI)
	float StartingCountdownRemaining = 3;

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
protected:
	UPROPERTY(BlueprintReadWrite)
	EGameState state = EGameState::Waiting;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = UpdateScoreUI)
	FGameScore Score;

	UFUNCTION(NetMulticast, reliable)
	void BroadcastGameFinished(FGameScore result);

	UFUNCTION(NetMulticast, reliable)
	void BroadcastGameStarting();

	UFUNCTION(NetMulticast, reliable)
	void BroadcastGameStarted();

};
