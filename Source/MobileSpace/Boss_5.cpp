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
		BossMesh->SetRelativeScale3D(FVector(0.9f, 0.9f, 0.9f)); // Tamaño medio
	}

	// Configuración específica para Boss_5 - Especialista en ráfagas
	BossHealth = 1700;
	CurrentHealth = BossHealth;
	FireRate = 0.8f; // Ráfagas muy rápidas
	AttackPattern = EAtackPattern::BossTargeted; // Nuevo patrón dirigido

	// Configurar efectos de aparición para Boss_5 (plasma/púrpura)
	static ConstructorHelpers::FObjectFinder<UParticleSystem> AppearanceEffectAsset(
		TEXT("ParticleSystem'/Game/MFK/Particles/Expanders/Par_ExpFire_01_Rain.Par_ExpFire_01_Rain'"));
	
	if (AppearanceEffectAsset.Succeeded())
	{
		AppearanceEffect = AppearanceEffectAsset.Object;
	}

	// Efecto de destrucción plasma para Boss_5
	static ConstructorHelpers::FObjectFinder<UParticleSystem> DestructionEffectAsset(
		TEXT("ParticleSystem'/Game/MFK/Particles/SawSpark/Par_FW_Saw_01_Rain.Par_FW_Saw_01_Rain'"));
	
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

