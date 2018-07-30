// Fill out your copyright notice in the Description page of Project Settings.

#include "PhysicsBall.h"


// Sets default values
APhysicsBall::APhysicsBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetReplicateMovement(true);
	SetReplicates(true);
}

// Called when the game starts or when spawned
void APhysicsBall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APhysicsBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//
//void APhysicsBall::SetVelocity(FVector velocity)
//{
//}
