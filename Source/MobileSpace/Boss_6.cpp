// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss_6.h"
#include "UObject/ConstructorHelpers.h"

ABoss_6::ABoss_6()
{
	PrimaryActorTick.bCanEverTick = true;

	// Cargar la malla SM_CF2_Shuttle1 desde BOSSES (versión mejorada del shuttle)
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BossMeshAsset(
		TEXT("StaticMesh'/Game/BOSSES/SM_CF2_Shuttle1.SM_CF2_Shuttle1'"));

	if (BossMeshAsset.Succeeded() && BossMesh)
	{
		BossMesh->SetStaticMesh(BossMeshAsset.Object);
		BossMesh->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.5f)); // ¡MASIVO! Jefe final
	}

	// Configuración específica para Boss_6 - JEFE FINAL
	BossHealth = 3500; // ¡La mayor vida!
	CurrentHealth = BossHealth;
	FireRate = 0.6f; // Disparos devastadores constantes
	AttackPattern = EAtackPattern::BossSpiral; // Empezar con espiral normal

	// Configurar movimiento de jefe final épico
	if (MoveComp)
	{
		MoveComp->Pattern = EArcadeMovement::BossEpicFinal; // ¡MOVIMIENTO ÉPICO FINAL!
		MoveComp->Speed = 60.0f; // Lento pero imponente
		MoveComp->Amplitude = 150.0f; // Movimiento controlado
		MoveComp->Frequency = 0.2f; // Muy lento y majestuoso
	}

	// Ajustar colisión para jefe final
	if (BossCollision)
	{
		BossCollision->SetBoxExtent(FVector(300.0f, 300.0f, 150.0f)); // Hitbox masiva
	}

	// Empezar en fase normal
	bEpicPhase = false;
}

void ABoss_6::BeginPlay()
{
	Super::BeginPlay();
	
	// Empezar con 6 segundos de pausa, luego alternar
	GetWorld()->GetTimerManager().SetTimer(SpecialAttackTimerHandle, this, &ABoss_6::SwitchAttackPhase, 6.0f, false);
	
	UE_LOG(LogTemp, Warning, TEXT("¡JEFE FINAL ACTIVO! 6s pausa → 3s épico → 6s pausa → 3s épico..."));
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
			
			UE_LOG(LogTemp, Warning, TEXT("💤 PAUSA - 6 segundos de respiro"));
		}, 3.0f, false);
		
		UE_LOG(LogTemp, Warning, TEXT("🔥🔥🔥 ÉPICO - 3 SEGUNDOS DE DEVASTACIÓN EXTREMA 🔥🔥🔥"));
	}
}

void ABoss_6::SpecialAttack()
{
	// Esta función ya no se usa, pero la mantengo por compatibilidad
}

void ABoss_6::DestroyBoss()
{
	// Limpiar timers específicos del Boss_6
	GetWorld()->GetTimerManager().ClearTimer(EpicPhaseTimerHandle);
	
	// Llamar destrucción padre
	Super::DestroyBoss();
}

