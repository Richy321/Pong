// Fill out your copyright notice in the Description page of Project Settings.

#include "GoalTriggerVolume.h"
#include "PhysicsBall.h"
#include "PongBlueprintFunctionLibrary.h"
#include "PongGameMode.h"
#include "Components/ShapeComponent.h"

AGoalTriggerVolume::AGoalTriggerVolume()
{
	if (Role == ENetRole::ROLE_Authority)
	{
		GetCollisionComponent()->OnComponentBeginOverlap.AddDynamic(this, &AGoalTriggerVolume::HandleComponentBeginOverlap);
	}
}

void AGoalTriggerVolume::BeginPlay()
{
}

void AGoalTriggerVolume::HandleComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Role == ENetRole::ROLE_Authority)
	{
		if (OtherActor->IsA<APhysicsBall>())
		{
			APongGameMode* gameMode = UPongBlueprintFunctionLibrary::GetPongGameMode(this);
			gameMode->IncrementScore(side == ESides::Left ? ESides::Right : ESides::Left);
		}
		else
		{
			UPongBlueprintFunctionLibrary::AddOnScreenDebugMessage(OtherActor->GetName() + TEXT("Is not a APhysicsBall"));
		}
	}
}