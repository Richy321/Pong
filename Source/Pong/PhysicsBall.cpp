// Fill out your copyright notice in the Description page of Project Settings.

#include "PhysicsBall.h"


// Sets default values
APhysicsBall::APhysicsBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetReplicateMovement(true);
	SetReplicates(true);

	ProjectileMoveComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMoveComponent0"));
	ProjectileMoveComponent->bShouldBounce = true;
	ProjectileMoveComponent->InitialSpeed = 325.0f;
	ProjectileMoveComponent->MaxSpeed = 500.0f;
	ProjectileMoveComponent->ProjectileGravityScale = 0.0f;
	ProjectileMoveComponent->Friction = 0.0f;
	ProjectileMoveComponent->Bounciness = 1.0f;
	ProjectileMoveComponent->SetPlaneConstraintEnabled(true);
	ProjectileMoveComponent->SetPlaneConstraintNormal(FVector(1.0f, 0.0f, 0.0f));
	ProjectileMoveComponent->SetPlaneConstraintOrigin(FVector(0.0f, 0.0f, 0.0f));
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
