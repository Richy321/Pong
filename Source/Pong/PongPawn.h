// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PongPawn.generated.h"

UCLASS()
class PONG_API APongPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APongPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, Category="Pong")
		class UStaticMeshComponent* MainMeshComponent;

public:	
	UFUNCTION(BlueprintCallable)
		void VerticalMovement(float MovementDelta);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float MovementSpeed = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UCurveFloat* ReflectionCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxBounceAngle = 75.0f; //in degrees

	float GetPaddleHeight() { return PaddleHeight; }
	float GetPaddleWidth() { return PaddleWidth; }

private:
	UFUNCTION()
	void HandleOnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	float PaddleHeight;
	float PaddleWidth;
};
