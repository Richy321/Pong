// Fill out your copyright notice in the Description page of Project Settings.

#include "PongHUD.h"

APongHUD::APongHUD()
{
	MenuManager = CreateDefaultSubobject<UMenuManager>(TEXT("MenuManager0"));
}

void APongHUD::BeginPlay()
{
	Super::BeginPlay();
	MenuManager->Initialise();
}