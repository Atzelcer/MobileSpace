// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss_1.h"
#include "UObject/ConstructorHelpers.h"

ABoss_1::ABoss_1()
{
	PrimaryActorTick.bCanEverTick = true;

	// Cargar la malla Intrepid_Type1 desde BOSSES
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BossMeshAsset(
		TEXT("StaticMesh'/Game/BOSSES/Intrepid_Type1.Intrepid_Type1'"));

	if (BossMeshAsset.Succeeded() && BossMesh)
	{
		BossMesh->SetStaticMesh(BossMeshAsset.Object);
		BossMesh->SetRelativeScale3D(FVector(5.2f, 5.2f, 5.2f));
		
	}
	
	static ConstructorHelpers::FObjectFinder<USoundBase> AppearanceSoundAsset(
		TEXT("/Game/BOSS_SOUNDS/EveryTime.EveryTime"));

	if (AppearanceSoundAsset.Succeeded())
	{
		AppearanceSound = AppearanceSoundAsset.Object;
	}
	
	// Configurar entrada épica
	EntranceHeight = 800.0f;
	EntranceSpeed = 200.0f;
	EntranceDuration = 4.0f;

	// Configuración específica para Boss_1
	BossHealth = 1500;
	CurrentHealth = BossHealth;
	FireRate = 1.8f;
	AttackPattern = EAtackPattern::Spread;

	// Configurar movimiento específico
	if (MoveComp)
	{
		MoveComp->Speed = 120.0f;
		MoveComp->Amplitude = 300.0f;
		MoveComp->Frequency = 0.4f;
	}

	// Configurar efectos de destrucción
	DestructionEffectScale = FVector(3.0f, 3.0f, 3.0f);
	
	// Cargar sonido de destrucción
	static ConstructorHelpers::FObjectFinder<USoundBase> DestructionSoundAsset(
		TEXT("/Game/BOSS_SOUNDS/EXPLO_BOSS.EXPLO_BOSS"));
	
	if (DestructionSoundAsset.Succeeded())
	{
		DestructionSound = DestructionSoundAsset.Object;
	}
}

