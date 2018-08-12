// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MenuManager.generated.h"

class UMenuScreenWidget;

/**
 * 
 */
UCLASS()
class PONG_API UMenuManager : public UObject
{
	GENERATED_BODY()

public:
	void Initialise();

	UFUNCTION(BlueprintCallable)
	void GoToScreen(EMenuScreens Screen);

	void HandleMovement(float DeltaMovement);
	void HandleSelect();
	void HandleBack();

protected:
	UPROPERTY(EditAnywhere)
	TMap<EMenuScreens, TSubclassOf<UMenuScreenWidget>> ScreenClasses;
	UPROPERTY(BlueprintReadOnly)
	EMenuScreens CurrentScreen;

private:
	UPROPERTY()
	TMap<EMenuScreens, UMenuScreenWidget*> Screens;
	template <typename T>
	void CreateMenu(const TSubclassOf<T>& MenuClass, T*& Menu, int LayerIndex);
	void CreateMenus();
};
