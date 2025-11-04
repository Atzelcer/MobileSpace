// Fill out your copyright notice in the Description page of Project Settings.

#include "Ship_CazadorAzul.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "AtackComponent.h"
#include "Engine/Engine.h"
#include "TimerManager.h"

AShip_CazadorAzul::AShip_CazadorAzul()
{
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMeshAsset(TEXT("StaticMesh'/Game/StarSparrow/Meshes/Examples/SM_StarSparrow02.SM_StarSparrow02'"));
	if (ShipMeshAsset.Succeeded())
	{
		ShipMesh->SetStaticMesh(ShipMeshAsset.Object);
		ShipMesh->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f)); // Aumenté el tamaño
	}

	AttackComp = CreateDefaultSubobject<UAtackComponent>(TEXT("AttackComponent"));

	if (MoveComp)
	{
		MoveComp->Pattern = EArcadeMovement::SuperSequence;
		MoveComp->Speed = 700.f;
		MoveComp->Amplitude = 120.f;
	}
}

void AShip_CazadorAzul::BeginPlay()
{
	Super::BeginPlay();
	
	SetActorRotation(FRotator(0.0f, -180.0f, 0.0f));
	GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AShip_CazadorAzul::AutoFire, TimeBetweenShots, true, 3.0f);
}

void AShip_CazadorAzul::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AShip_CazadorAzul::AutoFire()
{
	if (AttackComp)
	{
		AttackComp->Fire(AttackPattern);

	}
}

void AShip_CazadorAzul::DestruirNave()
{
	Super::DestruirNave();
}
