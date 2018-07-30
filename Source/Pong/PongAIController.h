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

private:
	void MoveTowards(const FVector& TargetPosition);
	FVector OriginalPosition;
	

};
