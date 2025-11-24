// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss_3.h"
#include "UObject/ConstructorHelpers.h"

ABoss_3::ABoss_3()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BossMeshAsset(
		TEXT("StaticMesh'/Game/StarSparrow/Meshes/Examples/SM_StarSparrow07.SM_StarSparrow07'"));

	if (BossMeshAsset.Succeeded() && BossMesh)
	{
		BossMesh->SetStaticMesh(BossMeshAsset.Object);
		BossMesh->SetRelativeScale3D(FVector(1.4f, 1.4f, 1.4f)); 
	}

	BossHealth = 2800;
	CurrentHealth = BossHealth;
	FireRate = 1.2f; 
	AttackPattern = EAtackPattern::BossSpiral; 

	static ConstructorHelpers::FObjectFinder<USoundBase> AppearanceSoundAsset(
		TEXT("SoundWave'/Game/BOSS_SOUNDS/Roi.Roi'"));
	
	if (AppearanceSoundAsset.Succeeded())
	{
		AppearanceSound = AppearanceSoundAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> DestructionSoundAsset(
		TEXT("/Game/BOSS_SOUNDS/EXPLO_BOSS.EXPLO_BOSS"));
	
	if (DestructionSoundAsset.Succeeded())
	{
		DestructionSound = DestructionSoundAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> AppearanceEffectAsset(
		TEXT("ParticleSystem'/Game/MFK/Particles/Expanders/Par_ExpFire_01_Rain.Par_ExpFire_01_Rain'"));
	
	if (AppearanceEffectAsset.Succeeded())
	{
		AppearanceEffect = AppearanceEffectAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> DestructionEffectAsset(
		TEXT("ParticleSystem'/Game/MFK/Particles/Expanders/Par_ExpFire_01_Rain.Par_ExpFire_01_Rain'"));
	
	if (DestructionEffectAsset.Succeeded())
	{
		DestructionEffect = DestructionEffectAsset.Object;
	}
	
	DestructionEffectScale = FVector(4.0f, 4.0f, 4.0f);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> TrailAsset(
		TEXT("ParticleSystem'/Game/VFXSeries1/Particles/Tails/P_RocketTypeB.P_RocketTypeB'"));
	if (TrailAsset.Succeeded())
	{
		TrailEffect = TrailAsset.Object;
		TrailParticleComponent->SetRelativeRotation(FRotator(0.f, -180.f, 0.f));
		TrailParticleComponent->SetRelativeScale3D(FVector(4.0f, 4.0f, 4.0f));

	}

	TrailOffset = FVector(-400.0f, 0.0f, 0.0f);

	bTrailActiveOnSpawn = true;
	
	if (MoveComp)
	{
		MoveComp->Pattern = EArcadeMovement::BossThreatening; 
		MoveComp->Speed = 140.0f; 
		MoveComp->Amplitude = 350.0f;
		MoveComp->Frequency = 0.6f;
	}

	if (BossCollision)
	{
		BossCollision->SetBoxExtent(FVector(500.0f, 400.0f, 400.0f)); 
	}
}

