// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "SidesEnum.h"
#include "GameTypesEnum.h"
#include "GameFramework/PlayerStart.h"
#include "PongGameMode.generated.h"

class APhysicsBall;
/**
 * 
 */

UCLASS()
class PONG_API APongGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	APongGameMode();

	UFUNCTION(BlueprintCallable)
	void Init(class ACameraActor* MainCamera, FVector HalfwayLineStart, FVector HalfwayLineEnd);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> BallClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<APawn> AIPawnClass;
		
	UFUNCTION(BlueprintCallable)
	AActor* SpawnBall();

	UFUNCTION(BlueprintCallable)
	void ResetBall(AActor* Ball, ESides SpawnSide);

	UFUNCTION(BlueprintCallable)
	APongAIController* SpawnAI(ESides Side);

	UFUNCTION(BlueprintCallable)
	APhysicsBall* GetBall();

	bool ReadyToStartMatch_Implementation() override;
	bool ReadyToEndMatch_Implementation() override;

	//Rules
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int NumGoalsToWin = 10;

	void IncrementScore(ESides Side);

	UPROPERTY(BlueprintReadOnly)
	FVector SpawnLineStart;
	UPROPERTY(BlueprintReadOnly)
	FVector SpawnLineEnd;

	AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	APlayerStart* GetFreePlayerStart(EGameType GameType, ESides Side);

	UFUNCTION(BlueprintCallable)
	void StartGame();

	UFUNCTION(BlueprintCallable)
	void RestartGame();

	void PostLogin(APlayerController* NewPlayer) override;

	void Tick(float DeltaSeconds) override;

	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float StartingCountdown = 3;

private:
	UPROPERTY()
	APhysicsBall* Ball;

	UPROPERTY()
	APongAIController* AIController;

	bool CheckWinState(ESides &Side);

	FTimerHandle StartingCountdownTimerHandle;
	FTimerHandle JoinStartDelayTimerHandle;
	int GetRequiredPlayerCount();
};
