// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss_5.h"
#include "UObject/ConstructorHelpers.h"

ABoss_5::ABoss_5()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BossMeshAsset(
		TEXT("StaticMesh'/Game/StarSparrow/Meshes/Examples/SM_StarSparrow10.SM_StarSparrow10'"));

	if (BossMeshAsset.Succeeded() && BossMesh)
	{
		BossMesh->SetStaticMesh(BossMeshAsset.Object);
		BossMesh->SetRelativeScale3D(FVector(1.4f, 1.4f, 1.4f));
		BossMesh->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f)); 
	}

	BossHealth = 3600;
	CurrentHealth = BossHealth;
	FireRate = 1.7f; 
	AttackPattern = EAtackPattern::BossTargeted;

	static ConstructorHelpers::FObjectFinder<USoundBase> AppearanceSoundAsset(
		TEXT("SoundWave'/Game/BOSS_SOUNDS/Descons.Descons'"));
	
	if (AppearanceSoundAsset.Succeeded())
	{
		AppearanceSound = AppearanceSoundAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> DestructionSoundAsset(
		TEXT("SoundWave'/Game/MFK/Sounds/Explode/A_Explode_06.A_Explode_06'"));
	
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
	
	DestructionEffectScale = FVector(5.0f, 5.0f, 5.0f);

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
		MoveComp->Pattern = EArcadeMovement::BossErraticPower;
		MoveComp->Speed = 200.0f; 
		MoveComp->Amplitude = 400.0f;
		MoveComp->Frequency = 0.7f;
	}

	if (BossCollision)
	{
		BossCollision->SetBoxExtent(FVector(500.0f, 600.0f, 400.0f)); 
	}
}

