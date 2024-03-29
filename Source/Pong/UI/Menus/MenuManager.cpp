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
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s - Trying to go to a screen without a set widget"), *FString(__FUNCTION__));
	}

	CurrentScreen = Screen;
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
		UMenuScreenWidget* NewMenuScreen;
		CreateMenu<UMenuScreenWidget>(MenuType.Value, NewMenuScreen, 10);
		Screens.Add(MenuType.Key, NewMenuScreen);
	}
}

void UMenuManager::HandleSelect()
{
	if (Screens.Contains(CurrentScreen) && IsValid(Screens[CurrentScreen]))
	{
		Screens[CurrentScreen]->SelectHighlighted();
	}
}

void UMenuManager::HandleBack()
{
	if (Screens.Contains(CurrentScreen) && IsValid(Screens[CurrentScreen]))
	{
		Screens[CurrentScreen]->Back();
	}
}
void UMenuManager::HandleMovement(float DeltaMovement)
{
	if (Screens.Contains(CurrentScreen) && IsValid(Screens[CurrentScreen]))
	{
		if (FMath::IsNearlyEqual(DeltaMovement, 0.0f))
		{
			return;
		}

		if (DeltaMovement > 0)
		{
			Screens[CurrentScreen]->HighlightPrevious();
		}
		else
		{
			Screens[CurrentScreen]->HighlightNext();
		}
	}
}

UMenuScreenWidget* UMenuManager::GetScreenWidget(EMenuScreens Screen)
{
	if (Screens.Contains(Screen) && IsValid(Screens[Screen]))
	{
		return Screens[Screen];
	}
	return nullptr;
}
