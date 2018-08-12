// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "UI/Menus/MenuManager.h"

#include "PongLobbyHUD.generated.h"

class UMenuScreenWidget;

UCLASS()
class PONG_API APongLobbyHUD : public AHUD
{
	GENERATED_BODY()
public:

	APongLobbyHUD();


	void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, Instanced, BlueprintReadOnly)
	UMenuManager* MenuManager;
};
