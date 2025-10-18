// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UMoveComponent::UMoveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	CurrentPattern = EMovementPattern::Idle;
	MovementTime = 0.0f;
	PatrolDirection = 1;
	

	// ...
}

void UMoveComponent::StartPattern(EMovementPattern Pattern)
{
	CurrentPattern = Pattern;
	MovementTime = 0.0f;

	AActor* Owner = GetOwner();
	if (Owner)
	{
		StartPosition = Owner->GetActorLocation();
	}
}

void UMoveComponent::StopMovemento()
{
	CurrentPattern = EMovementPattern::Idle;
}

// Called when the game starts
void UMoveComponent::BeginPlay()
{
	Super::BeginPlay();
	// Guardar posición inicial como formación si no se ha establecido
	AActor* Owner = GetOwner();
	if (Owner && StartPosition.IsZero())
	{
		StartPosition = Owner->GetActorLocation();
	}

	// ...
	
}



void UMoveComponent::UpdatePatrol(float DeltaTime)
{
	AActor* Owner = GetOwner();
	if (!Owner) return;

	FVector CurrentPos = Owner->GetActorLocation();
	float MoveAmount = Speed * DeltaTime * PatrolDirection;
	FVector NewPos = CurrentPos + FVector(0.0f, MoveAmount, 0.0f);

	// Cambiar dirección
	if (FMath::Abs(NewPos.Y - StartPosition.Y) >= Amplitude)
	{
		PatrolDirection *= -1;
	}

	Owner->SetActorLocation(NewPos);
}

void UMoveComponent::UpdateSineWave(float DeltaTime)
{
	AActor* Owner = GetOwner();
	if (!Owner) return;

	float X = StartPosition.X - Speed * MovementTime; // Avanza hacia la izquierda
	float Y = StartPosition.Y + FMath::Sin(MovementTime * Frequency) * Amplitude;

	Owner->SetActorLocation(FVector(X, Y, StartPosition.Z));
}

void UMoveComponent::UpdateFigure8(float DeltaTime)
{
	AActor* Owner = GetOwner();
	if (!Owner) return;

	float t = MovementTime * Frequency;
	float scale = Amplitude;

	// Ecuación paramétrica de figura 8
	float X = StartPosition.X + (scale * FMath::Sin(t));
	float Y = StartPosition.Y + (scale * FMath::Sin(t) * FMath::Cos(t));

	Owner->SetActorLocation(FVector(X, Y, StartPosition.Z));
}

void UMoveComponent::UpdateSpiral(float DeltaTime)
{
	AActor* Owner = GetOwner();
	if (!Owner) return;

	float t = MovementTime * Frequency;
	float radius = Amplitude * (1.0f - MovementTime / 5.0f); // Radio decrece con el tiempo

	float X = StartPosition.X - Speed * MovementTime * 0.3f;
	float Y = StartPosition.Y + FMath::Cos(t) * radius;
	float Z = StartPosition.Z; // Sin cambio en Z

	Owner->SetActorLocation(FVector(X, Y, Z));
}

void UMoveComponent::UpdateLoop(float DeltaTime)
{
	AActor* Owner = GetOwner();
	if (!Owner) return;

	float t = MovementTime * Frequency;

	float X = StartPosition.X + FMath::Cos(t) * Amplitude;
	float Y = StartPosition.Y + FMath::Sin(t) * Amplitude;

	Owner->SetActorLocation(FVector(X, Y, StartPosition.Z));
}

void UMoveComponent::UpdateZigZag(float DeltaTime)
{
	AActor* Owner = GetOwner();
	if (!Owner) return;

	float X = StartPosition.X - Speed * MovementTime;

	// Función triangular (onda cuadrada suavizada)
	float Y = StartPosition.Y + Amplitude * FMath::Sin(MovementTime * Frequency * 2.0f);

	Owner->SetActorLocation(FVector(X, Y, StartPosition.Z));
}

void UMoveComponent::UpdateParabola(float DeltaTime)
{
	AActor* Owner = GetOwner();
	if (!Owner) return;

	float t = MovementTime;

	// Ecuación parabólica: y = ax² + bx + c
	float X = StartPosition.X - Speed * t;
	float Y = StartPosition.Y + (Amplitude * 0.5f * t * t) - (Amplitude * t);

	Owner->SetActorLocation(FVector(X, Y, StartPosition.Z));
}

// Called every frame
void UMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	MovementTime += DeltaTime;

	switch (CurrentPattern)
	{
	case EMovementPattern::Patrol:
		UpdatePatrol(DeltaTime);
		break;
	case EMovementPattern::SineWave:
		UpdateSineWave(DeltaTime);
		break;
	case EMovementPattern::Figure8:
		UpdateFigure8(DeltaTime);
		break;
	case EMovementPattern::Spiral:
		UpdateSpiral(DeltaTime);
		break;
	case EMovementPattern::Loop:
		UpdateLoop(DeltaTime);
		break;
	case EMovementPattern::ZigZag:
		UpdateZigZag(DeltaTime);
		break;
	case EMovementPattern::Parabola:
		UpdateParabola(DeltaTime);
		break;
	default:
		break;
	}
	
}

