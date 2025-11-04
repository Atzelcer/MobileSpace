// Fill out your copyright notice in the Description page of Project Settings.

#include "Ship_CazadorRojo.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "AtackComponent.h"
#include "Engine/Engine.h"
#include "TimerManager.h"

AShip_CazadorRojo::AShip_CazadorRojo()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMeshAsset(TEXT("StaticMesh'/Game/StarSparrow/Meshes/Examples/SM_StarSparrow04.SM_StarSparrow04'"));
	if (ShipMeshAsset.Succeeded())
	{
		ShipMesh->SetStaticMesh(ShipMeshAsset.Object);
		ShipMesh->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));
	}

	AttackComp = CreateDefaultSubobject<UAtackComponent>(TEXT("AttackComponent"));

	if (MoveComp)
	{
		MoveComp->Pattern = EArcadeMovement::WavyWave;
		MoveComp->Speed = 400.f;
		MoveComp->Frequency = 1.7f;
		MoveComp->Amplitude = 210.f;
	}
}

void AShip_CazadorRojo::BeginPlay()
{
	Super::BeginPlay();
	
	SetActorRotation(FRotator(0.0f, -180.0f, 0.0f));
	
}

void AShip_CazadorRojo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AShip_CazadorRojo::AutoFire()
{
	if (AttackComp)
	{
		AttackComp->Fire(AttackPattern);
		
	}
}

void AShip_CazadorRojo::DestruirNave()
{
	Super::DestruirNave();
}
