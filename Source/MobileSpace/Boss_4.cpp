// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss_4.h"
#include "UObject/ConstructorHelpers.h"

ABoss_4::ABoss_4()
{
	PrimaryActorTick.bCanEverTick = true;

	// Cargar la malla SM_SkyFly_v08 desde BOSSES
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BossMeshAsset(
		TEXT("StaticMesh'/Game/BOSSES/SM_SkyFly_v08.SM_SkyFly_v08'"));

	if (BossMeshAsset.Succeeded() && BossMesh)
	{
		BossMesh->SetStaticMesh(BossMeshAsset.Object);
		BossMesh->SetRelativeScale3D(FVector(1.3f, 1.3f, 1.3f)); // Más grande = más fuerte
	}

	// Configuración específica para Boss_4 - Tanque pesado
	BossHealth = 2500;
	CurrentHealth = BossHealth;
	FireRate = 1.0f; // Dispara lento pero fuerte
	AttackPattern = EAtackPattern::BossWaveBarrage; // Nuevo patrón de ráfagas

	// Configurar movimiento pesado pero constante
	if (MoveComp)
	{
		MoveComp->Pattern = EArcadeMovement::BossCircularDominance; // Nuevo patrón dominante
		MoveComp->Speed = 80.0f; // Más lento pero resistente
		MoveComp->Amplitude = 200.0f;
		MoveComp->Frequency = 0.25f;
	}
}

