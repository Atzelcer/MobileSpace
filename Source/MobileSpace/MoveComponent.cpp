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

	case EArcadeMovement::Wave:
		// Onda suave en Y mientras avanza en X
		X -= Speed * Elapsed;
		Y += Amplitude * FMath::Sin(Frequency * Elapsed);
	break;

	case EArcadeMovement::WavyWave:
	{
		float tButterfly = 2.1f;
		float tTeleport = 0.4f;
		float tCurveReturn = 0.7f;
		FVector2D center = (MovementMin + MovementMax) * 0.5f;

		if (Elapsed < tButterfly)
		{
			float t = 2.0f * PI * Elapsed / tButterfly;
			X = center.X + 110.f * FMath::Sin(t) * (FMath::Exp(FMath::Cos(t)) - 2 * FMath::Cos(4 * t) - FMath::Sin(t / 12.f) * FMath::Sin(t / 12.f));
			Y = center.Y + 90.f * FMath::Cos(t);
		}
		else if (Elapsed < tButterfly + tTeleport)
		{
			float t = Elapsed - tButterfly;
			float destX = MovementMin.X + FMath::Fmod(Owner->GetUniqueID() * 147, int(MovementMax.X - MovementMin.X));
			float destY = MovementMin.Y + FMath::Fmod(Owner->GetUniqueID() * 71, int(MovementMax.Y - MovementMin.Y));
			X = FMath::Lerp(X, destX, t / tTeleport);
			Y = FMath::Lerp(Y, destY, t / tTeleport);
		}
		else if (Elapsed < tButterfly + tTeleport + tCurveReturn)
		{
			float t = Elapsed - tButterfly - tTeleport;
			// Vuelve curvando en espiral al centro
			X = FMath::Lerp(X, center.X, t / tCurveReturn) + 40.f * FMath::Cos(t * 4.5f);
			Y = FMath::Lerp(Y, center.Y, t / tCurveReturn) + 35.f * FMath::Sin(t * 3.2f);
		}
		else
		{
			Elapsed = 0.f;
		}
	}
	break;

	case EArcadeMovement::SEntry:
	{
		float tWave = 2.2f;
		float tJump = 0.4f;
		float tCurve = 0.8f;

		if (Elapsed < tWave)
		{
			float t = Elapsed;
			X = Origin.X + 180.f * FMath::Sin(Frequency * t + Owner->GetUniqueID());
			Y = Origin.Y + 160.f * FMath::Sin(0.7f * Frequency * t);
		}
		else if (Elapsed < tWave + tJump)
		{
			float t = Elapsed - tWave;
			// Salta a un borde superior/izq/der
			float edgeX = ((Owner->GetUniqueID() % 2) == 0 ? MovementMin.X : MovementMax.X);
			float edgeY = MovementMax.Y;
			X = FMath::Lerp(X, edgeX, t / tJump);
			Y = FMath::Lerp(Y, edgeY, t / tJump);
		}
		else if (Elapsed < tWave + tJump + tCurve)
		{
			float t = Elapsed - tWave - tJump;
			// Hace una pequeña curva elíptica de regreso
			X = X + 60.f * FMath::Cos(Frequency * t);
			Y = Y + 90.f * FMath::Sin(0.9f * t);
		}
		else
		{
			Elapsed = 0.f;
		}
	}
	break;

	case EArcadeMovement::ExpandingCircle:
	{
		float tOsc = 1.9f;
		float tCenter = 0.4f;
		float tSpiral = 1.1f;

		if (Elapsed < tOsc)
		{
			float t = Elapsed;
			X = Origin.X + 130.f * FMath::Cos(Frequency * t + Owner->GetUniqueID());
			Y = Origin.Y + 210.f * FMath::Sin(1.4f * Frequency * t - Owner->GetUniqueID());
		}
		else if (Elapsed < tOsc + tCenter)
		{
			float t = Elapsed - tOsc;
			// Teleport directo al centro del área
			FVector2D center = (MovementMin + MovementMax) * 0.5f;
			X = FMath::Lerp(X, center.X, t / tCenter);
			Y = FMath::Lerp(Y, center.Y, t / tCenter);
		}
		else if (Elapsed < tOsc + tCenter + tSpiral)
		{
			float t = Elapsed - tOsc - tCenter;
			// Espiral pequeña alrededor del centro
			float spiralRad = 70.f + 25.f * t;
			X = X + spiralRad * FMath::Sin(Frequency * t);
			Y = Y + spiralRad * FMath::Cos(Frequency * t);
		}
		else
		{
			Elapsed = 0.f;
		}
	}
	break;
	case EArcadeMovement::SuperSequence:
	{
		float tOnda = 2.0f;
		float tSaltoCentro = 0.6f;
		float tWarp = 0.5f;

		if (Elapsed < tOnda)
		{
			float t = Elapsed;
			X = Origin.X + 210.f * FMath::Sin(Frequency * t + Owner->GetUniqueID());
			Y = Origin.Y + 180.f * FMath::Sin((Frequency + 1.7f) * t);
		}
		else if (Elapsed < tOnda + tSaltoCentro)
		{
			float t = Elapsed - tOnda;
			// Salta directo (lerp rápido) al centro del área
			FVector2D center = (MovementMin + MovementMax) * 0.5f;
			X = FMath::Lerp(X, center.X, t / tSaltoCentro);
			Y = FMath::Lerp(Y, center.Y, t / tSaltoCentro);
		}
		else if (Elapsed < tOnda + tSaltoCentro + tWarp)
		{
			float t = Elapsed - tOnda - tSaltoCentro;
			// “Warpea” a un destino random distinto
			float destX = MovementMin.X + FMath::Fmod(Owner->GetUniqueID() * 99, int(MovementMax.X - MovementMin.X));
			float destY = MovementMin.Y + FMath::Fmod(Owner->GetUniqueID() * 183, int(MovementMax.Y - MovementMin.Y));
			X = FMath::Lerp(X, destX, t / tWarp);
			Y = FMath::Lerp(Y, destY, t / tWarp);
		}
		else
		{
			Elapsed = 0.f;
		}
	}
	break;
	case EArcadeMovement::RadialBurst:
	{
		float tSpiral = 1.7f;
		float tDiagonal = 0.5f;
		float tBounce = 0.7f;

		if (Elapsed < tSpiral)
		{
			float t = Elapsed;
			float radius = 120.f + t * 90.f;
			X = Origin.X + radius * FMath::Cos(Frequency * t + Owner->GetUniqueID());
			Y = Origin.Y + radius * FMath::Sin(Frequency * t + Owner->GetUniqueID());
		}
		else if (Elapsed < tSpiral + tDiagonal)
		{
			float t = Elapsed - tSpiral;
			// Salta en diagonal a la esquina contraria
			float destX = (Owner->GetUniqueID() % 2 == 0) ? MovementMax.X : MovementMin.X;
			float destY = (Owner->GetUniqueID() % 3 == 0) ? MovementMax.Y : MovementMin.Y;
			X = FMath::Lerp(X, destX, t / tDiagonal);
			Y = FMath::Lerp(Y, destY, t / tDiagonal);
		}
		else if (Elapsed < tSpiral + tDiagonal + tBounce)
		{
			float t = Elapsed - tSpiral - tDiagonal;
			// Rebote suave en esa esquina
			float bounce = 45.f * FMath::Sin(PI * t / tBounce);
			X += bounce;
			Y += bounce;
		}
		else
		{
			Elapsed = 0.f;
		}
	}
	break;

	default:
		X -= Speed * Elapsed;
		break;
	}

	X = FMath::Clamp(X, MovementMin.X, MovementMax.X);
	Y = FMath::Clamp(Y, MovementMin.Y, MovementMax.Y);

	Owner->SetActorLocation(FVector(X, Y, Z));
	
	
}



