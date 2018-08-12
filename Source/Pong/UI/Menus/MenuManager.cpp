// Fill out your copyright notice in the Description page of Project Settings.

#include "MenuManager.h"
#include "UI/Menus/MenuScreenWidget.h"

void UMenuManager::Initialise()
{
	CreateMenus();
}

void UMenuManager::GoToScreen(EMenuScreens Screen)
{
	if (Screens.Contains(CurrentScreen) && IsValid(Screens[CurrentScreen]))
	{
		Screens[CurrentScreen]->Hide();
	}

	if (Screens.Contains(Screen) && IsValid(Screens[Screen]))
	{
		Screens[Screen]->Show();
		CurrentScreen = Screen;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s - Trying to go to a screen without a set widget"), *FString(__FUNCTION__));
	}
}

template <typename T>
void UMenuManager::CreateMenu(const TSubclassOf<T>& MenuClass, T*& Menu, int LayerIndex)
{
	if (IsValid(MenuClass))
	{
		Menu = CreateWidget<T>(GetWorld(), MenuClass);
		if (IsValid(Menu))
		{
			Menu->AddToViewport(LayerIndex);
			Menu->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UMenuManager::CreateMenus()
{
	for (auto MenuType : ScreenClasses)
	{
		UMenuScreenWidget* newMenuScreen;
		CreateMenu<UMenuScreenWidget>(MenuType.Value, newMenuScreen, 10);
		Screens.Add(MenuType.Key, newMenuScreen);
	}
}
