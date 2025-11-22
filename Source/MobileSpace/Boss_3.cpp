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
		BossMesh->SetRelativeScale3D(FVector(5.1f, 5.1f, 5.1f)); // Tamaño medio-grande
	}

	// Configuración específica para Boss_3 - Más agresivo
	BossHealth = 2800;
	CurrentHealth = BossHealth;
	FireRate = 1.2f; // Dispara más rápido
	AttackPattern = EAtackPattern::BossSpiral; // Nuevo patrón en espiral

	// Sonido de aparición para Boss_3 (agresivo/ácido)
	static ConstructorHelpers::FObjectFinder<USoundBase> AppearanceSoundAsset(
		TEXT("SoundWave'/Game/BOSS_SOUNDS/Roi.Roi'"));
	
	if (AppearanceSoundAsset.Succeeded())
	{
		AppearanceSound = AppearanceSoundAsset.Object;
	}

	// Sonido de destrucción épico para Boss_3
	static ConstructorHelpers::FObjectFinder<USoundBase> DestructionSoundAsset(
		TEXT("/Game/BOSS_SOUNDS/EXPLO_BOSS.EXPLO_BOSS"));
	
	if (DestructionSoundAsset.Succeeded())
	{
		DestructionSound = DestructionSoundAsset.Object;
	}

	// Configurar efectos de aparición para Boss_3 (veneno/verde)
	static ConstructorHelpers::FObjectFinder<UParticleSystem> AppearanceEffectAsset(
		TEXT("ParticleSystem'/Game/MFK/Particles/Expanders/Halfs/Par_ExpFire_01_H_Rain.Par_ExpFire_01_H_Rain'"));
	
	if (AppearanceEffectAsset.Succeeded())
	{
		AppearanceEffect = AppearanceEffectAsset.Object;
	}

	// Efecto de destrucción verde para Boss_3
	static ConstructorHelpers::FObjectFinder<UParticleSystem> DestructionEffectAsset(
		TEXT("ParticleSystem'/Game/MFK/Particles_Tiny/Umbrella/Par_FW_Umbr_03_Tiny.Par_FW_Umbr_03_Tiny'"));
	
	if (DestructionEffectAsset.Succeeded())
	{
		DestructionEffect = DestructionEffectAsset.Object;
	}
	
	DestructionEffectScale = FVector(4.0f, 4.0f, 4.0f);

	// Configurar movimiento más dinámico
	if (MoveComp)
	{
		MoveComp->Pattern = EArcadeMovement::BossThreatening; // Nuevo patrón amenazante
		MoveComp->Speed = 140.0f; // Más rápido
		MoveComp->Amplitude = 350.0f;
		MoveComp->Frequency = 0.6f;
	}
}

