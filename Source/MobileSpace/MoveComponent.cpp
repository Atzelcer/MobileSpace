// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"

UMoveComponent::UMoveComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	Elapsed = 0.f;
}



void UMoveComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();
	if (Owner)
		Origin = Owner->GetActorLocation();
	
	
}




// Called every frame
void UMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	Elapsed += DeltaTime;
	AActor* Owner = GetOwner();
	if (!Owner) return;

	// Z siempre fijo en estilo top-down
	float Z = Origin.Z;

	float X = Origin.X;
	float Y = Origin.Y;

	switch (Pattern)
	{
	case EArcadeMovement::None:
		// Sin movimiento
		break;
	case EArcadeMovement::Slide:
		// Desliza recto por X
		X -= Speed * Elapsed;
		break;
	case EArcadeMovement::Wave:
		// Onda suave en Y mientras avanza en X
		X -= Speed * Elapsed;
		Y += Amplitude * FMath::Sin(Frequency * Elapsed);
		break;
	case EArcadeMovement::Spiral:
		// Espiral descentrado y avance en X
		X -= Speed * Elapsed * 0.6f + Amplitude * FMath::Cos(Frequency * Elapsed);
		Y += Amplitude * FMath::Sin(Frequency * Elapsed + Elapsed * 0.5f);
		break;
	case EArcadeMovement::RandomHop:
		// Movimiento recto + saltos random en Y
		X -= Speed * Elapsed;
		Y += Amplitude * FMath::Sin(Frequency * Elapsed + FMath::FRandRange(0.f, PI));
		break;
	case EArcadeMovement::Reverse:
		// Avanza en X y revierte dirección cada 2 segundos
	{
		float Dir = ((int)(Elapsed / 2) % 2 == 0) ? 1.f : -1.f;
		X += Dir * Speed * DeltaTime;
	}
	break;
	default:
		X -= Speed * Elapsed;
		break;
	}

	// Limitar a área (sin wrapping)
	X = FMath::Clamp(X, MovementMin.X, MovementMax.X);
	Y = FMath::Clamp(Y, MovementMin.Y, MovementMax.Y);

	Owner->SetActorLocation(FVector(X, Y, Z));
	
	
}



