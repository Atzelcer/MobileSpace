// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss_3.h"
#include "UObject/ConstructorHelpers.h"

ABoss_3::ABoss_3()
{
	PrimaryActorTick.bCanEverTick = true;

	// Cargar la malla SM_SkyFly_v07 desde BOSSES
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BossMeshAsset(
		TEXT("StaticMesh'/Game/BOSSES/SM_SkyFly_v07.SM_SkyFly_v07'"));

	if (BossMeshAsset.Succeeded() && BossMesh)
	{
		BossMesh->SetStaticMesh(BossMeshAsset.Object);
		BossMesh->SetRelativeScale3D(FVector(1.1f, 1.1f, 1.1f)); // Tamaño medio-grande
	}

	// Configuración específica para Boss_3 - Más agresivo
	BossHealth = 1800;
	CurrentHealth = BossHealth;
	FireRate = 1.2f; // Dispara más rápido
	AttackPattern = EAtackPattern::BossSpiral; // Nuevo patrón en espiral

	// Configurar movimiento más dinámico
	if (MoveComp)
	{
		MoveComp->Pattern = EArcadeMovement::BossThreatening; // Nuevo patrón amenazante
		MoveComp->Speed = 180.0f; // Más rápido
		MoveComp->Amplitude = 350.0f;
		MoveComp->Frequency = 0.6f;
	}
}

