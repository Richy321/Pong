// Fill out your copyright notice in the Description page of Project Settings.

#include "PongGameInstance.h"
#include "PongPlayerController.h"


UPongGameInstance::UPongGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

APongPlayerController* UPongGameInstance::GetPrimaryPlayerController()
{
	//TODO implement properly with login/engagement screen (xbox compat)
	APlayerController* pc = GetFirstLocalPlayerController(GetWorld());

	if (IsValid(pc))
	{
		return Cast<APongPlayerController>(pc);
	}

	return nullptr;
}