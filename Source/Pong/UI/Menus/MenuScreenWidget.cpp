// Fill out your copyright notice in the Description page of Project Settings.

#include "MenuScreenWidget.h"
#include "MenuButtonWidget.h"



void UMenuScreenWidget::SetHighlightIndex(int Index)
{
	if (MenuItems.IsValidIndex(Index))
	{
		if (MenuItems.IsValidIndex(HighlightedIndex))
		{
			MenuItems[HighlightedIndex]->UnHighlight();
		}

		MenuItems[HighlightedIndex]->Highlight();
		HighlightedIndex = Index;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s - Trying to highlight an invalid index"), *FString(__FUNCTION__));
	}
}

void UMenuScreenWidget::HighlightNext()
{
	if (MenuItems.IsValidIndex(HighlightedIndex + 1))
	{
		SetHighlightIndex(HighlightedIndex + 1);
	}
	else if (ShouldLoopAround && MenuItems.Num() > 0)
	{
		SetHighlightIndex(0);
	}
}

void UMenuScreenWidget::HighlightPrevious()
{
	if (MenuItems.IsValidIndex(HighlightedIndex - 1))
	{
		SetHighlightIndex(HighlightedIndex - 1);
	}
	else if (ShouldLoopAround && MenuItems.Num() > 0)
	{
		SetHighlightIndex(MenuItems.Num() - 1);
	}
}

void UMenuScreenWidget::SelectHighlighted()
{
	if (MenuItems.IsValidIndex(HighlightedIndex))
	{
		MenuItems[HighlightedIndex]->Select();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s - Trying to select an invalid index"), *FString(__FUNCTION__));

	}
}