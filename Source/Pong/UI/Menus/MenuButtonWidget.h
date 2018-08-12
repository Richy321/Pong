// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuButtonWidget.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMenuButtonSelect);

/**
 * 
 */
UCLASS()
class PONG_API UMenuButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void Highlight();
	
	UFUNCTION(BlueprintImplementableEvent)
	void UnHighlight();
	
	UFUNCTION(BlueprintCallable)
	void Select();

	UPROPERTY(BlueprintAssignable)
	FOnMenuButtonSelect OnSelect;
};
