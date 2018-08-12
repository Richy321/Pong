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
	if (bIsRepeatDelayActive)
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
	if (bIsRepeatDelayActive)
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

void UMenuScreenWidget::AddMenuButtonWidget(UMenuButtonWidget* button)
{
	MenuItems.Add(button);
}

void UMenuScreenWidget::ResetRepeatTimer()
{
	bIsRepeatDelayActive = true;
	GetWorld()->GetTimerManager().ClearTimer(RepeatDelayHandle);

	FTimerDelegate DeactivateRepeatDelayCallback;
	DeactivateRepeatDelayCallback.BindLambda([this]
	{
		bIsRepeatDelayActive = false;
	});

	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(RepeatDelayHandle, DeactivateRepeatDelayCallback, RepeatDelay, false);
}