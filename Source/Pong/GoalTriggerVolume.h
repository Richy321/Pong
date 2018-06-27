// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "SidesEnum.h"
#include "GoalTriggerVolume.generated.h"

/**
 * 
 */
UCLASS()
class PONG_API AGoalTriggerVolume : public ATriggerBox
{
	GENERATED_BODY()
	
public:
	AGoalTriggerVolume();

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	ESides side;

	void BeginPlay() override;

private:
	UFUNCTION()
	void HandleComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
