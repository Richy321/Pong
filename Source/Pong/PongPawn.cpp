// Fill out your copyright notice in the Description page of Project Settings.

#include "PongPawn.h"
#include "PhysicsBall.h"
#include "PongBlueprintFunctionLibrary.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Runtime/Engine/Classes/Curves/CurveFloat.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Components/StaticMeshComponent.h"

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

	if (HasAuthority())
	{
		SetReplicateMovement(true);
		SetReplicates(true);
	}

	if (IsValid(MainMeshComponent))
	{
		MainMeshComponent->OnComponentHit.AddDynamic(this, &APongPawn::HandleOnComponentHit);

		FBox Bounds = GetComponentsBoundingBox();
		PaddleHeight = Bounds.Max.Z - Bounds.Min.Z;
		PaddleWidth = Bounds.Max.Y - Bounds.Min.Y;
	}
}

void APongPawn::VerticalMovement(float MovementDelta)
{
	AddActorLocalOffset(FVector(0.0f, 0.0f, GetWorld()->GetDeltaSeconds() * MovementDelta * MovementSpeed), true);
}

PRAGMA_DISABLE_OPTIMIZATION
void APongPawn::HandleOnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->IsA(APhysicsBall::StaticClass()))
	{
		APhysicsBall* Ball = Cast<APhysicsBall>(OtherActor);
		Ball->LastHitter = GetController();

		FVector LocalImpactPoint = HitComponent->GetComponentTransform().Inverse().TransformPosition(Hit.ImpactPoint);

		//get percentage of whole height
		float zPositionPerc = LocalImpactPoint.Z / (PaddleHeight*0.5f);

		//sample position from curve and invert due to curve shape
		float ReflectionScaler = 1.0f - ReflectionCurve->GetFloatValue(FMath::Abs(zPositionPerc));

		//add the sign back in and convert to rad
		float MaxReflectAngle = FMath::DegreesToRadians(MaxBounceAngle * FMath::Sign(zPositionPerc));

		float XPos = FMath::Cos(MaxReflectAngle * ReflectionScaler);
		float YPos = FMath::Sin(MaxReflectAngle * ReflectionScaler);

		float ImpluseScalar = OtherActor->GetVelocity().Size();

		UActorComponent* ActorComponent = OtherActor->GetComponentByClass(UProjectileMovementComponent::StaticClass());
		if (IsValid(ActorComponent))
		{
			UProjectileMovementComponent* ProjectileComponent = Cast<UProjectileMovementComponent>(ActorComponent);
			if (IsValid(ProjectileComponent))
			{
				FVector Direction = FVector(0.0f, XPos, YPos);
				Direction.Normalize();
				ProjectileComponent->Velocity = Direction * ImpluseScalar;
			}
		}
	}
}
PRAGMA_ENABLE_OPTIMIZATION