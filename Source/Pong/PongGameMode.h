// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "SidesEnum.h"
#include "GameTypesEnum.h"
#include "GameFramework/PlayerStart.h"
#include "PongSpawnPoint.h"
#include "PongGameMode.generated.h"

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
	void Init(class ACameraActor* mainCamera, FVector halfwayLineStart, FVector halfwayLineEnd);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> BallClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<APawn> AIPawnClass;
		
	UFUNCTION(BlueprintCallable)
	AActor* SpawnBall();

	UFUNCTION(BlueprintCallable)
	void ResetBall(AActor* ball, ESides spawnSide);

	UFUNCTION(BlueprintCallable)
	void SpawnAI(ESides side);

	UFUNCTION(BlueprintCallable)
	AActor* GetBall();

	bool ReadyToStartMatch_Implementation() override;
	bool ReadyToEndMatch_Implementation() override;

	//Rules
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int NumGoalsToWin = 5;

	void IncrementScore(ESides side);

	UPROPERTY(BlueprintReadOnly)
	FVector spawnLineStart;
	UPROPERTY(BlueprintReadOnly)
	FVector spawnLineEnd;

	AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	EGameType GameType = EGameType::OneVsOne;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	EMultiplayerGameType MultiplayerGameType = EMultiplayerGameType::SinglePlayer;

	APlayerStart* GetFreePlayerStart(EGameType gameType, ESides side);

	UFUNCTION(BlueprintCallable)
	void StartGame();
	
private:
	UPROPERTY()
	class APhysicsBall* ball;
	bool CheckWinState(ESides &side);

	TArray<FPongSpawnPoint> SpawnPoints;
};
