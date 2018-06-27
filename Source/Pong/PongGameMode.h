// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "SidesEnum.h"
#include "PongGameMode.generated.h"

/**
 * 
 */
class APhysicsBall;
class ACameraActor;

UCLASS()
class PONG_API APongGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	APongGameMode();

	UFUNCTION(BlueprintCallable)
	void Init(ACameraActor* mainCamera, FVector halfwayLineStart, FVector halfwayLineEnd);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> BallClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> PlayerPawnClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> AIPawnClass;
		
	UFUNCTION(BlueprintCallable)
		AActor* SpawnBall(ESides spawnSide);

	//UFUNCTION(BlueprintCallable)
	//void SpawnPlayer(int spawnIndex);

	UFUNCTION(BlueprintCallable)
	void SpawnAI();

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
	UPROPERTY(BlueprintReadOnly)
	ACameraActor* mainCamera;

	AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	//AActor* FindPlayerStart_Implementation(AController* Player, const FString& IncomingName = TEXT("")) override;


private:
	UPROPERTY()
	APhysicsBall* ball;
	bool CheckWinState(ESides &side);
};
