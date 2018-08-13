// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PongBlueprintFunctionLibrary.generated.h"

UCLASS()
class PONG_API UPongBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category = "Util", meta = (WorldContext = "WorldContextObject"))
		static class APongHUD* GetPongHUD(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "Util", meta = (WorldContext = "WorldContextObject"))
		static class APongLobbyHUD* GetPongLobbyHUD(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "Util", meta = (WorldContext = "WorldContextObject"))
		static class UPongGameInstance* GetPongGameInstance(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "Util", meta = (WorldContext = "WorldContextObject"))
		static class APongGameState* GetPongGameState(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "Util", meta = (WorldContext = "WorldContextObject"))
		static class APongGameMode* GetPongGameMode(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Util")
		static void AddOnScreenDebugMessage(FString Text, int Index = -1, float TimeToDisplay = 4.0f, FLinearColor Colour = FLinearColor::Red, bool NewerOnTop = true, FVector2D TextScale = FVector2D::UnitVector);

	template<typename T>
	static T* GetDervivedHUD(const UObject* WorldContextObject);
};
