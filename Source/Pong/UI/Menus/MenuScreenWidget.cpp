// Fill out your copyright notice in the Description page of Project Settings.

#include "MenuScreenWidget.h"
#include "MenuButtonWidget.h"
#include "TimerManager.h"


void UMenuScreenWidget::SetHighlightIndex(int Index)
{
	if (MenuItems.IsValidIndex(Index))
	{
		if (MenuItems.IsValidIndex(HighlightedIndex))
		{
			MenuItems[HighlightedIndex]->UnHighlight();
		}

		MenuItems[Index]->Highlight();
		HighlightedIndex = Index;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s - Trying to highlight an invalid index"), *FString(__FUNCTION__));
	}
}

void UMenuScreenWidget::HighlightNext()
{
	//early out if we are delaying between calls
	if (RepeatDelayTimer > 0)
	{
		return;
	}

	if (MenuItems.IsValidIndex(HighlightedIndex + 1))
	{
		SetHighlightIndex(HighlightedIndex + 1);
	}
	else if (ShouldLoopAround && MenuItems.Num() > 0)
	{
		SetHighlightIndex(0);
	}
	ResetRepeatTimer();
}

void UMenuScreenWidget::HighlightPrevious()
{
	//early out if we are delaying between calls
	if (RepeatDelayTimer > 0)
	{
		return;
	}

	if (MenuItems.IsValidIndex(HighlightedIndex - 1))
	{
		SetHighlightIndex(HighlightedIndex - 1);
	}
	else if (ShouldLoopAround && MenuItems.Num() > 0)
	{
		SetHighlightIndex(MenuItems.Num() - 1);
	}
	ResetRepeatTimer();
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

void UMenuScreenWidget::AddMenuButtonWidget(UMenuButtonWidget* Button)
{
	MenuItems.Add(Button);
}

void UMenuScreenWidget::ResetRepeatTimer()
{
	RepeatDelayTimer = RepeatDelay;
}

void UMenuScreenWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	//reduce repeat delay in tick instead of timer (Can't use timers whilst paused)
	if (RepeatDelayTimer > 0.0f)
	{
		RepeatDelayTimer = FMath::Max(0.0f, RepeatDelayTimer - InDeltaTime);
	}
	Super::NativeTick(MyGeometry, InDeltaTime);
}