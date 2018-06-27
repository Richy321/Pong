// Fill out your copyright notice in the Description page of Project Settings.

#include "PongPawn.h"
#include "Components/StaticMeshComponent.h"
#include "PongBlueprintFunctionLibrary.h"
#include "Runtime/Engine/Classes/Curves/CurveFloat.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PhysicsBall.h"

// Sets default values
APongPawn::APongPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APongPawn::BeginPlay()
{
	Super::BeginPlay();
	if (IsValid(mainMeshComponent))
	{
		mainMeshComponent->OnComponentHit.AddDynamic(this, &APongPawn::HandleOnComponentHit);

		FVector min, max;
		mainMeshComponent->GetLocalBounds(min, max);
		float paddleHeight = max.Z - min.Z;
		PaddleHeightHalf = paddleHeight / 2.0f;
	}
}

// Called every frame
void APongPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APongPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APongPawn::VerticalMovement(float Delta)
{
	AddActorLocalOffset(FVector(0.0f, 0.0f, Delta * MovementSpeed), true);
}

PRAGMA_DISABLE_OPTIMIZATION
void APongPawn::HandleOnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->IsA(APhysicsBall::StaticClass()))
	{
		FVector localImpactPoint = HitComponent->GetComponentTransform().Inverse().TransformPosition(Hit.ImpactPoint);

		//get percentage of whole height
		float zPositionPerc = localImpactPoint.Z / PaddleHeightHalf;

		//sample position from curve and invert
		float reflectionScaler = 1.0f - ReflectionCurve->GetFloatValue(FMath::Abs(zPositionPerc));

		//add the sign back in and convert to rad
		float maxReflectAngle = FMath::DegreesToRadians(MaxBounceAngle * FMath::Sign(zPositionPerc));


		float xPos = FMath::Cos(maxReflectAngle * reflectionScaler);
		float yPos = FMath::Sin(maxReflectAngle * reflectionScaler);

		float impluseScalar = OtherActor->GetVelocity().Size();

		UActorComponent* actorComponent = OtherActor->GetComponentByClass(UProjectileMovementComponent::StaticClass());
		if (IsValid(actorComponent))
		{
			UProjectileMovementComponent* projectileComponent = Cast<UProjectileMovementComponent>(actorComponent);
			if (IsValid(projectileComponent))
			{
				FVector direction = FVector(0.0f, xPos, yPos);
				direction.Normalize();
				projectileComponent->Velocity = direction * impluseScalar;
				UPongBlueprintFunctionLibrary::AddOnScreenDebugMessage(FString::Printf(TEXT("Velocity: %s"), *projectileComponent->Velocity.ToCompactString()));
			}
		}
	}
}
PRAGMA_ENABLE_OPTIMIZATION