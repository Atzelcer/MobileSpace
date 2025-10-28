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
	CurrentPattern = EMovementPattern::Linear;
	MovementTime = 0.0f;
	PatrolDirection = 1;
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
	CurrentPattern = EMovementPattern::Linear;
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

void UMoveComponent::UpdateLinear(float DeltaTime)
{
	AActor* Owner = GetOwner();
	if (!Owner) return;

	// Movimiento constante hacia la izquierda (hacia el jugador)
	float X = StartPosition.X - Speed * MovementTime;
	float Y = StartPosition.Y; // Sin cambio en Y - perfectamente recto
	
	FVector DesiredPosition = FVector(X, Y, StartPosition.Z);
	FVector FinalPosition = ApplyScreenBounds(DesiredPosition);
	Owner->SetActorLocation(FinalPosition);
}

// 2. MOVIMIENTO ONDULATORIO - Seno suave
void UMoveComponent::UpdateSineWave(float DeltaTime)
{
	AActor* Owner = GetOwner();
	if (!Owner) return;

	// Movimiento hacia la izquierda con ondas senoidales suaves
	//float Frequency = 2.0f; // Frecuencia de las ondas
	//float Amplitude = 80.0f; // Altura de las ondas (suave)
	
	float X = StartPosition.X - Speed * MovementTime;
	float Y = StartPosition.Y + Amplitude * FMath::Sin(Frequency * MovementTime);
	
	FVector DesiredPosition = FVector(X, Y, StartPosition.Z);
	FVector FinalPosition = ApplyScreenBounds(DesiredPosition);
	Owner->SetActorLocation(FinalPosition);
}

// 3. MOVIMIENTO CIRCULAR - Círculo perfecto
void UMoveComponent::UpdateCircular(float DeltaTime)
{
	AActor* Owner = GetOwner();
	if (!Owner) return;

	// Círculo matemático perfecto
	float Radius = 100.0f; // Radio del círculo
	float AngularSpeed = 3.0f; // Velocidad angular
	
	float Angle = AngularSpeed * MovementTime;
	float X = StartPosition.X - Speed * MovementTime * 0.3f + Radius * FMath::Cos(Angle);
	float Y = StartPosition.Y + Radius * FMath::Sin(Angle);
	
	FVector DesiredPosition = FVector(X, Y, StartPosition.Z);
	FVector FinalPosition = ApplyScreenBounds(DesiredPosition);
	Owner->SetActorLocation(FinalPosition);
}

// 4. MOVIMIENTO ZIGZAG - Dientes de sierra matemáticos
void UMoveComponent::UpdateZigZag(float DeltaTime)
{
	AActor* Owner = GetOwner();
	if (!Owner) return;

	// ZigZag usando función diente de sierra (triangular)
	//float Frequency = 2.0f;
	//float Amplitude = 120.0f;
	
	float SawtoothValue = (2.0f / PI) * FMath::Asin(FMath::Sin(PI * Frequency * MovementTime));
	
	float X = StartPosition.X - Speed * MovementTime;
	float Y = StartPosition.Y + Amplitude * SawtoothValue;
	
	FVector DesiredPosition = FVector(X, Y, StartPosition.Z);
	FVector FinalPosition = ApplyScreenBounds(DesiredPosition);
	Owner->SetActorLocation(FinalPosition);
}

// 5. MOVIMIENTO ELÍPTICO - Elipse matemática
void UMoveComponent::UpdateElliptical(float DeltaTime)
{
	AActor* Owner = GetOwner();
	if (!Owner) return;

	// Elipse con diferentes radios en X e Y
	float RadiusX = 80.0f;  // Radio horizontal
	float RadiusY = 140.0f; // Radio vertical (más grande para efecto elíptico)
	float AngularSpeed = 2.5f;
	
	float Angle = AngularSpeed * MovementTime;
	float X = StartPosition.X - Speed * MovementTime * 0.4f + RadiusX * FMath::Cos(Angle);
	float Y = StartPosition.Y + RadiusY * FMath::Sin(Angle);
	
	FVector DesiredPosition = FVector(X, Y, StartPosition.Z);
	FVector FinalPosition = ApplyScreenBounds(DesiredPosition);
	Owner->SetActorLocation(FinalPosition);
}

FVector UMoveComponent::ApplyScreenBounds(const FVector& DesiredPosition)
{
	if (!bUseBounds)
		return DesiredPosition;

	FVector WrapPosition = DesiredPosition;
	
	// WRAP-AROUND HORIZONTAL (X) - Si sale por cualquier lado, SIEMPRE aparece ARRIBA EN EL LADO DERECHO
	if (WrapPosition.X < MinX || WrapPosition.X > MaxX)
	{
		// Reaparece desde la parte superior derecha (como en los clásicos)
		WrapPosition.X = MaxX - 200.0f; // Un poco adentro del borde derecho
		WrapPosition.Y = MaxY - 100.0f;  // Desde arriba
		
		// Reiniciar posición inicial para nuevos cálculos de movimiento
		StartPosition = WrapPosition;
		MovementTime = 0.0f;
		
		UE_LOG(LogTemp, Warning, TEXT("Nave WRAP! Nueva posición: X=%f, Y=%f"), WrapPosition.X, WrapPosition.Y);
	}
	
	// WRAP-AROUND VERTICAL (Y) - Si sale por arriba o abajo, aparece del lado opuesto
	else if (WrapPosition.Y < MinY)
	{
		WrapPosition.Y = MaxY - 50.0f; // Aparece por arriba
		StartPosition.Y = WrapPosition.Y;
	}
	else if (WrapPosition.Y > MaxY)
	{
		WrapPosition.Y = MinY + 50.0f; // Aparece por abajo  
		StartPosition.Y = WrapPosition.Y;
	}
	
	// Z se mantiene igual (altura)
	return WrapPosition;
}

// Called every frame
void UMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	MovementTime += DeltaTime;

	switch (CurrentPattern)
	{
	case EMovementPattern::Linear:
		UpdateLinear(DeltaTime);
		break;
	case EMovementPattern::SineWave:
		UpdateSineWave(DeltaTime);
		break;
	case EMovementPattern::Circular:
		UpdateCircular(DeltaTime);
		break;
	case EMovementPattern::ZigZag:
		UpdateZigZag(DeltaTime);
		break;
	case EMovementPattern::Elliptical:
		UpdateElliptical(DeltaTime);
		break;
	default:
		UpdateLinear(DeltaTime); // Por defecto movimiento lineal
		break;
	}
	
}



