// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss_5.h"
#include "UObject/ConstructorHelpers.h"

ABoss_5::ABoss_5()
{
	PrimaryActorTick.bCanEverTick = true;

	// Cargar la malla tempes desde BOSSES
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BossMeshAsset(
		TEXT("StaticMesh'/Game/BOSSES/tempes.tempes'"));

	if (BossMeshAsset.Succeeded() && BossMesh)
	{
		BossMesh->SetStaticMesh(BossMeshAsset.Object);
		BossMesh->SetRelativeScale3D(FVector(4.9f, 4.9f, 4.9f)); // Tamaño medio
		BossMesh->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f)); // Rotar para mirar hacia adelante
	}

	// Configuración específica para Boss_5 - Especialista en ráfagas
	BossHealth = 3600;
	CurrentHealth = BossHealth;
	FireRate = 1.7f; // Ráfagas muy rápidas
	AttackPattern = EAtackPattern::BossTargeted; // Nuevo patrón dirigido

	// Sonido de aparición para Boss_5 (rápido/plasma)
	static ConstructorHelpers::FObjectFinder<USoundBase> AppearanceSoundAsset(
		TEXT("SoundWave'/Game/BOSS_SOUNDS/Descons.Descons'"));
	
	if (AppearanceSoundAsset.Succeeded())
	{
		AppearanceSound = AppearanceSoundAsset.Object;
	}

	// Sonido de destrucción explosivo para Boss_5 (rápido)
	static ConstructorHelpers::FObjectFinder<USoundBase> DestructionSoundAsset(
		TEXT("SoundWave'/Game/MFK/Sounds/Explode/A_Explode_06.A_Explode_06'"));
	
	if (DestructionSoundAsset.Succeeded())
	{
		DestructionSound = DestructionSoundAsset.Object;
	}

	// Configurar efectos de aparición para Boss_5 (plasma/púrpura)
	static ConstructorHelpers::FObjectFinder<UParticleSystem> AppearanceEffectAsset(
		TEXT("ParticleSystem'/Game/MFK/Particles/Expanders/Par_ExpFire_01_Rain.Par_ExpFire_01_Rain'"));
	
	if (AppearanceEffectAsset.Succeeded())
	{
		AppearanceEffect = AppearanceEffectAsset.Object;
	}

	// Efecto de destrucción plasma para Boss_5
	static ConstructorHelpers::FObjectFinder<UParticleSystem> DestructionEffectAsset(
		TEXT("ParticleSystem'/Game/MFK/Particles_Tiny/SawSpark/Par_FW_Saw_01_Rain_Tiny.Par_FW_Saw_01_Rain_Tiny'"));
	
	if (DestructionEffectAsset.Succeeded())
	{
		DestructionEffect = DestructionEffectAsset.Object;
	}
	
	DestructionEffectScale = FVector(5.0f, 5.0f, 5.0f);

	// Configurar movimiento errático
	if (MoveComp)
	{
		MoveComp->Pattern = EArcadeMovement::BossErraticPower; // Nuevo patrón errático
		MoveComp->Speed = 200.0f; // Muy móvil
		MoveComp->Amplitude = 400.0f;
		MoveComp->Frequency = 0.7f;
	}
}

