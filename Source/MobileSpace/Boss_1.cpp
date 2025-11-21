// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss_1.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

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

	// Configurar efectos de aparición para Boss_1 (azul/hielo)
	static ConstructorHelpers::FObjectFinder<UParticleSystem> AppearanceEffectAsset(
		TEXT("ParticleSystem'/Game/MFK/Particles_Tiny/FlatRings/Par_FW_FlatRing_03_Rain_Tiny.Par_FW_FlatRing_03_Rain_Tiny'"));
	
	if (AppearanceEffectAsset.Succeeded())
	{
		AppearanceEffect = AppearanceEffectAsset.Object;
	}

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
	
	// Efecto de destrucción azul para Boss_1
	static ConstructorHelpers::FObjectFinder<UParticleSystem> DestructionEffectAsset(
		TEXT("ParticleSystem'/Game/MFK/Particles_Tiny/Umbrella/Par_FW_Umbr_03_Tiny.Par_FW_Umbr_03_Tiny'"));
	
	if (DestructionEffectAsset.Succeeded())
	{
		DestructionEffect = DestructionEffectAsset.Object;
	}
	
	// Cargar sonido de destrucción
	static ConstructorHelpers::FObjectFinder<USoundBase> DestructionSoundAsset(
		TEXT("/Game/BOSS_SOUNDS/EXPLO_BOSS.EXPLO_BOSS"));
	
	if (DestructionSoundAsset.Succeeded())
	{
		DestructionSound = DestructionSoundAsset.Object;
	}
}

void ABoss_1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Solo rotar si no está en entrada épica
	if (!bIsEntering)
	{
		// Buscar al jugador
		APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if (Player)
		{
			// Calcular dirección hacia el jugador
			FVector PlayerLocation = Player->GetActorLocation();
			FVector BossLocation = GetActorLocation();
			FVector Direction = PlayerLocation - BossLocation;
			Direction.Z = 0.0f; // Mantener rotación solo en el plano XY
			
			// Convertir dirección a rotación
			FRotator TargetRotation = Direction.Rotation();
			
			// Rotar suavemente hacia el jugador
			FRotator CurrentRotation = GetActorRotation();
			FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 2.0f);
			SetActorRotation(NewRotation);
		}
	}
}

