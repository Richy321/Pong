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

	void HandleMovement(float MovementDelta);
	void HandleSelect();
	void HandleBack();

	UFUNCTION(BlueprintCallable)
	void VerticalMovement(float MovementDelta);
	UFUNCTION(Server, Unreliable, WithValidation, BlueprintCallable)
	void ServerVerticalMovement(float MovementDelta);
	UFUNCTION(NetMulticast, Unreliable)
	void BroadcastVerticalMovement(float MovementDelta);

	UFUNCTION(BlueprintCallable)
	void TogglePause();
	UFUNCTION(BlueprintCallable)
	void LeaveGame();

	void MenuMovement(float MovementDelta, class UMenuScreenWidget* MenuWidget);

	void SetupInputComponent() override;

	UFUNCTION(BlueprintCallable)
	void RequestRematch();
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRequestRematch();

private:
	//UFUNCTION()
	//	void Pause();
	//UFUNCTION(Server, Reliable, WithValidation)
	//	void ServerPause();
	//UFUNCTION(NetMulticast, Reliable)
	//	void BroadcastPause();

	//UFUNCTION()
	//	void UnPause();
	//UFUNCTION(Server, Reliable, WithValidation)
	//	void ServerUnPause();
	//UFUNCTION(NetMulticast, Reliable)
	//	void BroadcastUnPause();
};