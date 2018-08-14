// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PhysicsBall.generated.h"

UCLASS()
class PONG_API APhysicsBall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APhysicsBall();

	const UProjectileMovementComponent& GetProjectileMovementComponent() { return *ProjectileMoveComponent; }
	
	virtual void Tick(float DeltaTime) override;

	//Last player controller to hit the ball. nullptr indicates a serve
	UPROPERTY()
	AController* LastHitter;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UProjectileMovementComponent* ProjectileMoveComponent;
};
