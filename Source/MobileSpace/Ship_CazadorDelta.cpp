// Fill out your copyright notice in the Description page of Project Settings.

#include "Ship_CazadorDelta.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "AtackComponent.h"
#include "Engine/Engine.h"
#include "TimerManager.h"

AShip_CazadorDelta::AShip_CazadorDelta()
{
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMeshAsset(TEXT("StaticMesh'/Game/StarSparrow/Meshes/Examples/SM_StarSparrow09.SM_StarSparrow09'"));
	if (ShipMeshAsset.Succeeded())
	{
		ShipMesh->SetStaticMesh(ShipMeshAsset.Object);
		ShipMesh->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));
	}

	// Crear componente de ataque (MoveComp ya se hereda del padre)
	AttackComp = CreateDefaultSubobject<UAtackComponent>(TEXT("AttackComponent"));
}

void AShip_CazadorDelta::BeginPlay()
{
	Super::BeginPlay();
	
	// Configurar rotación inicial
	SetActorRotation(FRotator(0.0f, -180.0f, 0.0f));
	
	// Configurar movimiento - Delta usa movimiento zigzag
	MoveComp->StartPosition = GetActorLocation();
	MoveComp->StartPattern(EMovementPattern::ZigZag);
	
	// Configurar disparo automático con ráfagas
	GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AShip_CazadorDelta::AutoFire, TimeBetweenShots, true, 1.5f);
}

void AShip_CazadorDelta::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AShip_CazadorDelta::AutoFire()
{
	if (AttackComp)
	{
		AttackComp->Fire(AttackPattern);
		//if (GEngine)
		//{
		//	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, TEXT("Cazador Delta Fired Burst!"));
		//}
	}
}
