// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "SidesEnum.h"
#include "GameScore.h"
#include "UI/Menus/MenuScreenEnum.h"
#include "UI/Menus/MenuManager.h"
#include "PongHUD.generated.h"

class UMenuScreenWidget;

UCLASS()
class PONG_API APongHUD : public AHUD
{
	GENERATED_BODY()
public:

	APongHUD();
	UFUNCTION(BlueprintImplementableEvent)
	void SetScore(const FGameScore& Score);
	UFUNCTION(BlueprintImplementableEvent)
	void ShowGameFinishedUI(const FGameScore Result);

	void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, Instanced, BlueprintReadOnly)
	UMenuManager* MenuManager;
};
