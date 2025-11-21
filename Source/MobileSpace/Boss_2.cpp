// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss_2.h"
#include "UObject/ConstructorHelpers.h"

ABoss_2::ABoss_2()
{
	PrimaryActorTick.bCanEverTick = true;

	// Cargar la malla SM_CF2_Shuttle desde BOSSES
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BossMeshAsset(
		TEXT("StaticMesh'/Game/BOSSES/SM_CF2_Shuttle.SM_CF2_Shuttle'"));

	if (BossMeshAsset.Succeeded() && BossMesh)
	{
		BossMesh->SetStaticMesh(BossMeshAsset.Object);
		BossMesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f)); // Tamaño medio
	}

	// Configuración específica para Boss_2
	BossHealth = 2000;
	CurrentHealth = BossHealth;
	FireRate = 1.5f;
	AttackPattern = EAtackPattern::BossCircular; // Nuevo patrón circular

	// Configurar efectos de aparición para Boss_2 (fuego/rojo)
	static ConstructorHelpers::FObjectFinder<UParticleSystem> AppearanceEffectAsset(
		TEXT("ParticleSystem'/Game/MFK/Particles/Expanders/Par_ExpFire_01_Rain.Par_ExpFire_01_Rain'"));
	
	if (AppearanceEffectAsset.Succeeded())
	{
		AppearanceEffect = AppearanceEffectAsset.Object;
	}

	// Efecto de destrucción rojo para Boss_2
	static ConstructorHelpers::FObjectFinder<UParticleSystem> DestructionEffectAsset(
		TEXT("ParticleSystem'/Game/MFK/Particles/Expanders/Par_ExpFire_01_Rain.Par_ExpFire_01_Rain'"));
	
	if (DestructionEffectAsset.Succeeded())
	{
		DestructionEffect = DestructionEffectAsset.Object;
	}
	
	DestructionEffectScale = FVector(3.5f, 3.5f, 3.5f);

	// Configurar movimiento específico - Nuevo patrón elegante
	if (MoveComp)
	{
		MoveComp->Pattern = EArcadeMovement::BossSlowSweep; // Nuevo movimiento elegante
		MoveComp->Speed = 100.0f;
		MoveComp->Amplitude = 250.0f;
		MoveComp->Frequency = 0.3f;
	}
}

