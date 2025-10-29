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

	// Crear componente de ataque (MoveComp ya se hereda del padre)
	AttackComp = CreateDefaultSubobject<UAtackComponent>(TEXT("AttackComponent"));
}

void AShip_CazadorRojo::BeginPlay()
{
	Super::BeginPlay();
	
	// Configurar rotación inicial
	SetActorRotation(FRotator(0.0f, -180.0f, 0.0f));
	
	// Configurar movimiento - Rojo usa loop circular
	MoveComp->StartPosition = GetActorLocation();
	MoveComp->StartPattern(EMovementPattern::SineWave);
	
	// Configurar disparo automático con spread
	GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AShip_CazadorRojo::AutoFire, TimeBetweenShots, true, 2.0f);
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
		//if (GEngine)
		//{
		//	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Cazador Rojo Fired Spread!"));
		//}
	}
}
