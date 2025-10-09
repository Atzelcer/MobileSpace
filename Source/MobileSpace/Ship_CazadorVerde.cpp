// Fill out your copyright notice in the Description page of Project Settings.


#include "Ship_CazadorVerde.h"
#include "UObject/ConstructorHelpers.h"

AShip_CazadorVerde::AShip_CazadorVerde()
{
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMeshAsset(TEXT("StaticMesh'/Game/StarSparrow/Meshes/Examples/SM_StarSparrow05.SM_StarSparrow05'"));
	if (ShipMeshAsset.Succeeded())
	{
		ShipMesh->SetStaticMesh(ShipMeshAsset.Object);
		ShipMesh->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));
	}

	// Set specific properties for this ship type
	MoveSpeed = 1200.0f; // Slightly faster attack speed
	HorizontalSpeed = 1800.0f; // Faster horizontal movement than base
	AttackTime = 0.0f;
	
	// Slightly different limits for variation
	LeftLimit = -750.0f;
	RightLimit = 750.0f;
}

void AShip_CazadorVerde::BeginPlay()
{
	Super::BeginPlay();
	
	// Start pattern cycling timer
	GetWorld()->GetTimerManager().SetTimer(PatternChangeTimer, this, &AShip_CazadorVerde::CycleMovementPattern, 4.0f, true);
}

void AShip_CazadorVerde::CycleMovementPattern()
{
	static int32 PatternIndex = 0;
	
	// Reset all patterns
	ActivateNormalMode();
	
	// Activate new pattern
	switch (PatternIndex)
	{
		case 0:
			ActivateZigzagMode();
			UE_LOG(LogTemp, Warning, TEXT("🌟 CazadorVerde: LISSAJOUS CURVE - Figure 8 Infinity Pattern!"));
			break;
		case 1:
			ActivatePulseMode();
			UE_LOG(LogTemp, Warning, TEXT("🌸 CazadorVerde: ROSE CURVE - Mathematical Flower Petals!"));
			break;
		case 2:
			ActivateSerpentineMode();
			UE_LOG(LogTemp, Warning, TEXT("💖 CazadorVerde: CARDIOID - Heart-Shaped Curve!"));
			break;
		case 3:
			// Special fourth pattern - we'll handle this in UpdateMovement
			bZigzagMode = false;
			bPulseMode = false; 
			bSerpentineMode = false;
			bMovingDown = true; // Use this flag for the special pattern
			UE_LOG(LogTemp, Warning, TEXT("🌀 CazadorVerde: HYPOCYCLOID - Star Burst Pattern!"));
			break;
		default:
			UE_LOG(LogTemp, Warning, TEXT("➡️ CazadorVerde: NORMAL - Simple Horizontal!"));
			break;
	}
	
	PatternIndex = (PatternIndex + 1) % 5; // Cycle through 5 patterns now
}

void AShip_CazadorVerde::UpdateMovement(float DeltaTime)
{
	// Creative movement patterns in X-Y plane
	static float MovementTime = 0.0f;
	MovementTime += DeltaTime;
	
	if (bInFormation && !bAttacking)
	{
		FVector CurrentPos = GetActorLocation();
		
		// SPECTACULAR MATHEMATICAL PATTERNS - SUPER NOTICEABLE!
		if (bZigzagMode)
		{
			// LISSAJOUS CURVE PATTERN - Figure 8 + Infinity shape
			float FreqX = 1.5f; // X frequency
			float FreqY = 3.0f; // Y frequency (double for figure-8)
			float Amplitude = 400.0f; // Large amplitude for visibility
			
			CurrentPos.X = FormationPosition.X + FMath::Sin(MovementTime * FreqX) * Amplitude;
			CurrentPos.Y = FormationPosition.Y + FMath::Sin(MovementTime * FreqY) * (Amplitude * 0.6f);
			
			UE_LOG(LogTemp, Warning, TEXT("CazadorVerde: LISSAJOUS CURVE - Figure 8 pattern!"));
		}
		else if (bPulseMode)
		{
			// ROSE CURVE PATTERN - Mathematical flower petals
			float PetalCount = 5.0f; // Number of petals
			float RadiusBase = 300.0f;
			float Angle = MovementTime * 2.0f; // Rotation speed
			
			// Rose curve formula: r = a * cos(k * θ)
			float Radius = RadiusBase * FMath::Abs(FMath::Cos(PetalCount * Angle));
			
			CurrentPos.X = FormationPosition.X + Radius * FMath::Cos(Angle);
			CurrentPos.Y = FormationPosition.Y + Radius * FMath::Sin(Angle);
			
			UE_LOG(LogTemp, Warning, TEXT("CazadorVerde: ROSE CURVE - %f petal flower!"), PetalCount);
		}
		else if (bSerpentineMode)
		{
			// CARDIOID PATTERN - Heart-shaped mathematical curve
			float Scale = 200.0f;
			float T = MovementTime * 1.5f; // Parameter
			
			// Cardioid parametric equations: x = a(2cos(t) - cos(2t)), y = a(2sin(t) - sin(2t))
			float CosT = FMath::Cos(T);
			float SinT = FMath::Sin(T);
			float Cos2T = FMath::Cos(2.0f * T);
			float Sin2T = FMath::Sin(2.0f * T);
			
			CurrentPos.X = FormationPosition.X + Scale * (2.0f * CosT - Cos2T);
			CurrentPos.Y = FormationPosition.Y + Scale * (2.0f * SinT - Sin2T);
			
			UE_LOG(LogTemp, Warning, TEXT("CazadorVerde: CARDIOID - Heart-shaped curve!"));
		}
		else if (bMovingDown) // Special fourth pattern
		{
			// HYPOCYCLOID PATTERN - Star burst with multiple cusps
			float R = 250.0f; // Large circle radius
			float r = R / 4.0f; // Small circle radius (R/4 creates 4-pointed star)
			float T = MovementTime * 2.5f; // Parameter speed
			
			// Hypocycloid parametric equations: 
			// x = (R-r)*cos(t) + r*cos((R-r)/r * t)
			// y = (R-r)*sin(t) - r*sin((R-r)/r * t)
			float RMinusR = R - r;
			float Ratio = RMinusR / r;
			
			CurrentPos.X = FormationPosition.X + RMinusR * FMath::Cos(T) + r * FMath::Cos(Ratio * T);
			CurrentPos.Y = FormationPosition.Y + RMinusR * FMath::Sin(T) - r * FMath::Sin(Ratio * T);
			
			UE_LOG(LogTemp, Warning, TEXT("CazadorVerde: HYPOCYCLOID - 4-pointed star burst!"));
		}
		else
		{
			// NORMAL: Simple horizontal bounce - call parent
			Super::UpdateMovement(DeltaTime);
			return;
		}
		
		SetActorLocation(CurrentPos);
	}
	else if (bAttacking)
	{
		// Attack toward player (Y direction)
		AttackTime += DeltaTime;
		
		FVector CurrentLocation = GetActorLocation();
		FVector AttackDirection;
		
		// Curved attack in X-Y plane
		AttackDirection.X = FMath::Sin(AttackTime * 2.0f) * 50.0f * DeltaTime; // Side-to-side
		AttackDirection.Y = -MoveSpeed * DeltaTime; // Toward player
		AttackDirection.Z = 0.0f; // No height change
		
		FVector NewLocation = CurrentLocation + AttackDirection;
		SetActorLocation(NewLocation);
		
		// Return to formation after going off screen
		if (NewLocation.Y < -1200.0f)
		{
			ReturnToFormation();
			AttackTime = 0.0f;
		}
	}
}

void AShip_CazadorVerde::StartAttackPattern()
{
	// Reset attack time when starting new attack
	AttackTime = 0.0f;
	
	// Call parent implementation
	Super::StartAttackPattern();
}
