// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss_4.h"
#include "UObject/ConstructorHelpers.h"

ABoss_4::ABoss_4()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BossMeshAsset(
		TEXT("StaticMesh'/Game/StarSparrow/Meshes/Examples/SM_StarSparrow20.SM_StarSparrow20'"));

	if (BossMeshAsset.Succeeded() && BossMesh)
	{
		BossMesh->SetStaticMesh(BossMeshAsset.Object);
		BossMesh->SetRelativeScale3D(FVector(1.4f, 1.4f, 1.4f)); 
	}

	BossHealth = 2900;
	CurrentHealth = BossHealth;
	FireRate = 1.0f; 
	AttackPattern = EAtackPattern::BossSpiral; 

	static ConstructorHelpers::FObjectFinder<USoundBase> AppearanceSoundAsset(
		TEXT("SoundWave'/Game/BOSS_SOUNDS/FLUX.FLUX'"));
	
	if (AppearanceSoundAsset.Succeeded())
	{
		AppearanceSound = AppearanceSoundAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> DestructionSoundAsset(
		TEXT("/Game/BOSS_SOUNDS/EX_GALAGAEXPLOTION.EX_GALAGAEXPLOTION"));
	
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
	
	DestructionEffectScale = FVector(4.5f, 4.5f, 4.5f);

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
		MoveComp->Pattern = EArcadeMovement::BossCircularDominance;
		MoveComp->Speed = 80.0f; 
		MoveComp->Amplitude = 200.0f;
		MoveComp->Frequency = 0.25f;
	}
	if (BossCollision)
	{
		BossCollision->SetBoxExtent(FVector(550.0f, 450.0f, 450.0f)); 
	}
}

