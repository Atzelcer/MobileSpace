// Fill out your copyright notice in the Description page of Project Settings.

#include "Ship_CazadorAlfa.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "AtackComponent.h"
#include "Engine/Engine.h"
#include "TimerManager.h"

AShip_CazadorAlfa::AShip_CazadorAlfa()
{
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMeshAsset(TEXT("StaticMesh'/Game/StarSparrow/Meshes/Examples/SM_StarSparrow16.SM_StarSparrow16'"));
	if (ShipMeshAsset.Succeeded())
	{
		ShipMesh->SetStaticMesh(ShipMeshAsset.Object);
		ShipMesh->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));
	}

	// Crear componente de ataque (MoveComp ya se hereda del padre)
	AttackComp = CreateDefaultSubobject<UAtackComponent>(TEXT("AttackComponent"));
}

void AShip_CazadorAlfa::BeginPlay()
{
	Super::BeginPlay();
	
	// Configurar rotación inicial
	SetActorRotation(FRotator(0.0f, -180.0f, 0.0f));
	
	// Configurar movimiento - Alfa usa movimiento recto simple
	MoveComp->StartPosition = GetActorLocation();
	MoveComp->StartPattern(EMovementPattern::ZigZag);
	
	// Configurar disparo automático
	GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AShip_CazadorAlfa::AutoFire, TimeBetweenShots, true, 1.0f);
}

void AShip_CazadorAlfa::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AShip_CazadorAlfa::AutoFire()
{
	if (AttackComp)
	{
		AttackComp->Fire(AttackPattern);
	}
}
