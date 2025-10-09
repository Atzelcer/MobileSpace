// Fill out your copyright notice in the Description page of Project Settings.


#include "Ship_X.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "TimerManager.h"

// Sets default values
AShip_X::AShip_X()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Create and setup the mesh component
	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	RootComponent = ShipMesh;
	
	// Initialize default values
	MoveSpeed = 500.0f;
	HorizontalSpeed = 500.0f;
	bInFormation = false;
	bAttacking = false;
	FormationPosition = FVector::ZeroVector;
	
	// Initialize movement limits and direction (X-Y plane)
	bMovingRight = true;
	LeftLimit = -800.0f;
	RightLimit = 800.0f;
	FrontLimit = 200.0f; // Y positive (toward player)
	BackLimit = 800.0f;  // Y negative (away from player)
	
	// Initialize creative movement flags
	bMovingDown = false;
	bZigzagMode = false;
	bPulseMode = false;
	bSerpentineMode = false;
	VerticalSpeed = 100.0f;
	PulseIntensity = 50.0f;
	ZigzagCounter = 0;
	ZigzagSteps = 3;
}

// Called when the game starts or when spawned
void AShip_X::BeginPlay()
{
	Super::BeginPlay();
	
	// Start in formation behavior
	JoinFormation();
}

// Called every frame
void AShip_X::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Update movement based on current state
	UpdateMovement(DeltaTime);
}

void AShip_X::UpdateMovement(float DeltaTime)
{
	// Base class: Simple horizontal movement only (X-Y plane)
	if (bInFormation && !bAttacking)
	{
		FVector CurrentPos = GetActorLocation();
		
		// Simple horizontal movement on X axis
		if (bMovingRight)
		{
			CurrentPos.X += HorizontalSpeed * DeltaTime;
			if (CurrentPos.X >= RightLimit)
			{
				bMovingRight = false;
			}
		}
		else
		{
			CurrentPos.X -= HorizontalSpeed * DeltaTime;
			if (CurrentPos.X <= LeftLimit)
			{
				bMovingRight = true;
			}
		}
		
		SetActorLocation(CurrentPos);
	}
	else if (bAttacking)
	{
		// Attack pattern: move toward player (Y axis toward 0)
		FVector CurrentLocation = GetActorLocation();
		FVector AttackDirection = FVector(0, -1, 0); // Move toward player on Y axis
		
		FVector NewLocation = CurrentLocation + AttackDirection * MoveSpeed * DeltaTime;
		SetActorLocation(NewLocation);
		
		// Return to formation after going off screen
		if (NewLocation.Y < -1000.0f)
		{
			ReturnToFormation();
		}
	}
}

void AShip_X::StartAttackPattern()
{
	bAttacking = true;
	bInFormation = false;
	
	// Schedule to return to formation after a delay
	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &AShip_X::ReturnToFormation, 5.0f, false);
}

void AShip_X::ReturnToFormation()
{
	bAttacking = false;
	bInFormation = true;
	
	// Clear any existing timer
	GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
}

void AShip_X::SetFormationPosition(FVector NewPosition)
{
	FormationPosition = NewPosition;
}

void AShip_X::JoinFormation()
{
	bInFormation = true;
	bAttacking = false;
}

void AShip_X::StartAttack()
{
	StartAttackPattern();
}

void AShip_X::ActivateZigzagMode()
{
	bZigzagMode = true;
	bPulseMode = false;
	bSerpentineMode = false;
	ZigzagCounter = 0;
}

void AShip_X::ActivatePulseMode()
{
	bZigzagMode = false;
	bPulseMode = true;
	bSerpentineMode = false;
}

void AShip_X::ActivateSerpentineMode()
{
	bZigzagMode = false;
	bPulseMode = false;
	bSerpentineMode = true;
}

void AShip_X::ActivateNormalMode()
{
	bZigzagMode = false;
	bPulseMode = false;
	bSerpentineMode = false;
}

