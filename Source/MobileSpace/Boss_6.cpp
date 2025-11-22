// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss_6.h"
#include "UObject/ConstructorHelpers.h"

ABoss_6::ABoss_6()
{
	PrimaryActorTick.bCanEverTick = true;

	// Cargar la malla SM_CF2_Shuttle1 desde BOSSES (versión mejorada del shuttle)
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BossMeshAsset(
		TEXT("StaticMesh'/Game/BOSSES/SM_CF2_Shuttle.SM_CF2_Shuttle'"));

	if (BossMeshAsset.Succeeded() && BossMesh)
	{
		BossMesh->SetStaticMesh(BossMeshAsset.Object);
		BossMesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	}

	// Configuración específica para Boss_6 - JEFE FINAL
	BossHealth = 5000; // Vida baja para testing rápido
	CurrentHealth = BossHealth;
	FireRate = 0.6f; // Disparos devastadores constantes
	AttackPattern = EAtackPattern::BossSpiral; // Empezar con espiral normal

	// Sonido de aparición ÉPICO para Boss_6 (jefe final)
	static ConstructorHelpers::FObjectFinder<USoundBase> AppearanceSoundAsset(
		TEXT("SoundWave'/Game/BOSS_SOUNDS/METAMO.METAMO'"));
	
	if (AppearanceSoundAsset.Succeeded())
	{
		AppearanceSound = AppearanceSoundAsset.Object;
	}

	// Sonido de destrucción TITANICO para Boss_6 (jefe final)
	static ConstructorHelpers::FObjectFinder<USoundBase> DestructionSoundAsset(
		TEXT("SoundWave'/Game/MFK/Sounds/Explode/A_Explode_06.A_Explode_06'"));
	
	if (DestructionSoundAsset.Succeeded())
	{
		DestructionSound = DestructionSoundAsset.Object;
	}

	// Configurar efectos de aparición ÉPICOS para Boss_6 (dorado/final)
	static ConstructorHelpers::FObjectFinder<UParticleSystem> AppearanceEffectAsset(
		TEXT("ParticleSystem'/Game/MFK/Particles_Tiny/Umbrella/Par_FW_Umbr_03_Tiny.Par_FW_Umbr_03_Tiny'"));
	
	if (AppearanceEffectAsset.Succeeded())
	{
		AppearanceEffect = AppearanceEffectAsset.Object;
	}

	// Efecto de destrucción ÉPICO para Boss_6
	static ConstructorHelpers::FObjectFinder<UParticleSystem> DestructionEffectAsset(
		TEXT("ParticleSystem'/Game/MFK/Particles_Tiny/Expanders/Par_ExpFire_02_Rain_Tiny.Par_ExpFire_02_Rain_Tiny'"));
	
	if (DestructionEffectAsset.Succeeded())
	{
		DestructionEffect = DestructionEffectAsset.Object;
	}
	
	DestructionEffectScale = FVector(10.0f, 10.0f, 10.0f); // ¡ÉPICO!

	// Configurar movimiento de jefe final épico
	if (MoveComp)
	{
		MoveComp->Pattern = EArcadeMovement::BossSlowSweep; // ¡MOVIMIENTO ÉPICO FINAL!
		MoveComp->Speed = 60.0f; // Lento pero imponente
		MoveComp->Amplitude = 150.0f; // Movimiento controlado
		MoveComp->Frequency = 0.2f; // Muy lento y majestuoso
	}

	// Configurar hitbox MÁS GRANDE para Boss_6 (jefe final)
	if (BossCollision)
	{
		BossCollision->SetBoxExtent(FVector(400.0f, 400.0f, 400.0f)); // ¡HITBOX MASIVA!
	}

	// Empezar en fase normal
	bEpicPhase = false;
}

void ABoss_6::BeginPlay()
{
	Super::BeginPlay();
	
	// Empezar con 6 segundos de pausa, luego alternar
	GetWorld()->GetTimerManager().SetTimer(SpecialAttackTimerHandle, this, &ABoss_6::SwitchAttackPhase, 6.0f, false);
	
}

void ABoss_6::SwitchAttackPhase()
{
	if (!bEpicPhase)
	{
		// Activar FASE ÉPICA por 3 segundos
		bEpicPhase = true;
		AttackPattern = EAtackPattern::BossEpicFinal;
		FireRate = 0.3f; // ¡MUY RÁPIDO durante épico!
		
		// Timer para terminar fase épica después de 3 segundos
		GetWorld()->GetTimerManager().SetTimer(EpicPhaseTimerHandle, [this]()
		{
			// Terminar fase épica y empezar pausa de 6 segundos
			bEpicPhase = false;
			AttackPattern = EAtackPattern::Single; // Sin ataques durante pausa
			FireRate = 10.0f; // Prácticamente sin disparos
			
			// Programar próxima fase épica en 6 segundos
			GetWorld()->GetTimerManager().SetTimer(SpecialAttackTimerHandle, this, &ABoss_6::SwitchAttackPhase, 6.0f, false);
			
		}, 3.0f, false);
		
	}
}

