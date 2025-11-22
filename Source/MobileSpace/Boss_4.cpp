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
		BossMesh->SetRelativeScale3D(FVector(5.3f, 5.3f, 5.3f)); // Más grande = más fuerte
	}

	// Configuración específica para Boss_4 - Tanque pesado
	BossHealth = 2900;
	CurrentHealth = BossHealth;
	FireRate = 1.0f; // Dispara lento pero fuerte
	AttackPattern = EAtackPattern::BossSpiral; // Nuevo patrón de ráfagas

	// Sonido de aparición para Boss_4 (pesado/eléctrico)
	static ConstructorHelpers::FObjectFinder<USoundBase> AppearanceSoundAsset(
		TEXT("SoundWave'/Game/BOSS_SOUNDS/FLUX.FLUX'"));
	
	if (AppearanceSoundAsset.Succeeded())
	{
		AppearanceSound = AppearanceSoundAsset.Object;
	}

	// Sonido de destrucción poderoso para Boss_4 (tanque pesado)
	static ConstructorHelpers::FObjectFinder<USoundBase> DestructionSoundAsset(
		TEXT("/Game/BOSS_SOUNDS/EX_GALAGAEXPLOTION.EX_GALAGAEXPLOTION"));
	
	if (DestructionSoundAsset.Succeeded())
	{
		DestructionSound = DestructionSoundAsset.Object;
	}

	// Configurar efectos de aparición para Boss_4 (eléctrico/amarillo)
	static ConstructorHelpers::FObjectFinder<UParticleSystem> AppearanceEffectAsset(
		TEXT("ParticleSystem'/Game/MFK/Particles/Expanders/Par_ExpFire_01_Rain.Par_ExpFire_01_Rain'"));
	
	if (AppearanceEffectAsset.Succeeded())
	{
		AppearanceEffect = AppearanceEffectAsset.Object;
	}

	// Efecto de destrucción eléctrico para Boss_4
	static ConstructorHelpers::FObjectFinder<UParticleSystem> DestructionEffectAsset(
		TEXT("ParticleSystem'/Game/MFK/Particles_Tiny/SawSpark/Par_FW_Saw_01_Rain_Tiny.Par_FW_Saw_01_Rain_Tiny'"));
	
	if (DestructionEffectAsset.Succeeded())
	{
		DestructionEffect = DestructionEffectAsset.Object;
	}
	
	DestructionEffectScale = FVector(4.5f, 4.5f, 4.5f);

	// Configurar movimiento pesado pero constante
	if (MoveComp)
	{
		MoveComp->Pattern = EArcadeMovement::BossCircularDominance; // Nuevo patrón dominante
		MoveComp->Speed = 80.0f; // Más lento pero resistente
		MoveComp->Amplitude = 200.0f;
		MoveComp->Frequency = 0.25f;
	}
}

