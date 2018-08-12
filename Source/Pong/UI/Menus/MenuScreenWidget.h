// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuButtonWidget.h"
#include "MenuScreenWidget.generated.h"

UCLASS()
class PONG_API UMenuScreenWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void Show();
	UFUNCTION(BlueprintImplementableEvent)
	void Hide();
	UFUNCTION(BlueprintImplementableEvent)
	void Back();

	UFUNCTION(BlueprintCallable)
	void SetHighlightIndex(int Index);
	UFUNCTION(BlueprintCallable)
	inline int GetHighlightedIndex() { return HighlightedIndex; }

	UFUNCTION(BlueprintCallable)
	void HighlightNext();
	UFUNCTION(BlueprintCallable)
	void HighlightPrevious();

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool ShouldLoopAround = true;
	
	UFUNCTION(BlueprintCallable)
	void AddMenuButtonWidget(UMenuButtonWidget* button);

	void SelectHighlighted();

protected:


	UPROPERTY(BlueprintReadOnly)
	int HighlightedIndex = 0;

	UPROPERTY(EditAnywhere)
	float RepeatDelay = 0.2f;

private:
	TArray<UMenuButtonWidget*> MenuItems;

	void ResetRepeatTimer();
	FTimerHandle RepeatDelayHandle;
	bool bIsRepeatDelayActive = false;
};
