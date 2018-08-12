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
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UMenuButtonWidget*> MenuItems;

	void SelectHighlighted();

protected:
	UPROPERTY(BlueprintReadOnly)
	int HighlightedIndex = 0;
};
