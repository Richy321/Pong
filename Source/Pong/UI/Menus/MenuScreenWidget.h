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
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Show();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Hide();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
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

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	UPROPERTY(BlueprintReadOnly)
	int HighlightedIndex = 0;

	UPROPERTY(EditAnywhere)
	float RepeatDelay = 0.2f;

private:
	TArray<UMenuButtonWidget*> MenuItems;

	void ResetRepeatTimer();
	float RepeatDelayTimer = 0.0f;
};
