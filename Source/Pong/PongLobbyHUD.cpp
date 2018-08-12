// Fill out your copyright notice in the Description page of Project Settings.

#include "PongLobbyHUD.h"

APongLobbyHUD::APongLobbyHUD()
{
	MenuManager = CreateDefaultSubobject<UMenuManager>(TEXT("MenuManagerLobby0"));
}

void APongLobbyHUD::BeginPlay()
{
	//initialise menu manager before calling blueprint code (Super::BeginPlay)
	MenuManager->Initialise();
	Super::BeginPlay();
}