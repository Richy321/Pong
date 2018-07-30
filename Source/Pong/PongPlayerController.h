// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "PongPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PONG_API APongPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	APongPlayerController();

	void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
		void VerticalMovement(float MovementDelta);

	UFUNCTION(Server, Unreliable, WithValidation, BlueprintCallable)
		void ServerVerticalMovement(float MovementDelta);

	UFUNCTION(NetMulticast, Unreliable)
		void BroadcastVerticalMovement(float MovementDelta);

	void SetupInputComponent() override;
};
