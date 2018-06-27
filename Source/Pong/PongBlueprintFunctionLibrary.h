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
		static class UPongGameInstance* GetPongGameInstance(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "Util", meta = (WorldContext = "WorldContextObject"))
		static class APongGameState* GetPongGameState(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "Util", meta = (WorldContext = "WorldContextObject"))
		static class APongGameMode* GetPongGameMode(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Util")
		static void AddOnScreenDebugMessage(FString text, int index = -1, float timeToDisplay = 4.0f, FLinearColor colour = FLinearColor::Red, bool newerOnTop = true, FVector2D textScale = FVector2D::UnitVector);

};


#define UE_LOG_FUNC(text) UE_LOG(LogTemp, Warning, TEXT("%s - ") + , *FString(__FUNCTION__), controllerID, *errorStr);