// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss_2.h"
#include "UObject/ConstructorHelpers.h"

ABoss_2::ABoss_2()
{
	PrimaryActorTick.bCanEverTick = true;

	// Cargar la malla SM_CF2_Shuttle desde BOSSES
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BossMeshAsset(
		TEXT("StaticMesh'/Game/BOSSES/fast_texturing_output.fast_texturing_output'"));

	if (BossMeshAsset.Succeeded() && BossMesh)
	{
		BossMesh->SetStaticMesh(BossMeshAsset.Object);
		BossMesh->SetRelativeScale3D(FVector(4.0f, 4.0f, 4.0f)); // Tamaño medio
	}

	// Configuración específica para Boss_2
	BossHealth = 2800;
	CurrentHealth = BossHealth;
	FireRate = 1.5f;
	AttackPattern = EAtackPattern::BossCircular; // Nuevo patrón circular

	// Sonido de aparición para Boss_2 (fuego/potente)
	static ConstructorHelpers::FObjectFinder<USoundBase> AppearanceSoundAsset(
		TEXT("/Game/BOSS_SOUNDS/ApeerBoss01.ApeerBoss01"));
	
	if (AppearanceSoundAsset.Succeeded())
	{
		AppearanceSound = AppearanceSoundAsset.Object;
	}

	// Sonido de destrucción épico para Boss_2
	static ConstructorHelpers::FObjectFinder<USoundBase> DestructionSoundAsset(
		TEXT("/Game/BOSS_SOUNDS/BOSS02_EXPLO.BOSS02_EXPLO"));
	
	if (DestructionSoundAsset.Succeeded())
	{
		DestructionSound = DestructionSoundAsset.Object;
	}

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

