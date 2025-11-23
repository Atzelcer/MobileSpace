// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss_2.h"
#include "UObject/ConstructorHelpers.h"

ABoss_2::ABoss_2()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BossMeshAsset(
		TEXT("StaticMesh'/Game/StarSparrow/Meshes/Examples/SM_StarSparrow06.SM_StarSparrow06'"));

	if (BossMeshAsset.Succeeded() && BossMesh)
	{
		BossMesh->SetStaticMesh(BossMeshAsset.Object);
		BossMesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	}
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
	BossHealth = 2800;
	CurrentHealth = BossHealth;
	FireRate = 1.5f;
	AttackPattern = EAtackPattern::BossCircular;

	static ConstructorHelpers::FObjectFinder<USoundBase> AppearanceSoundAsset(
		TEXT("SoundWave'/Game/BOSS_SOUNDS/FLUX.FLUX'"));
	if (AppearanceSoundAsset.Succeeded()) AppearanceSound = AppearanceSoundAsset.Object;

	static ConstructorHelpers::FObjectFinder<USoundBase> DestructionSoundAsset(
		TEXT("/Game/BOSS_SOUNDS/BOSS02_EXPLO.BOSS02_EXPLO"));
	if (DestructionSoundAsset.Succeeded()) DestructionSound = DestructionSoundAsset.Object;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> AppearanceEffectAsset(
		TEXT("ParticleSystem'/Game/MFK/Particles/Expanders/Par_ExpFire_01_Rain.Par_ExpFire_01_Rain'"));
	if (AppearanceEffectAsset.Succeeded()) AppearanceEffect = AppearanceEffectAsset.Object;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> DestructionEffectAsset(
		TEXT("ParticleSystem'/Game/MFK/Particles/Expanders/Par_ExpFire_01_Rain.Par_ExpFire_01_Rain'"));
	if (DestructionEffectAsset.Succeeded()) DestructionEffect = DestructionEffectAsset.Object;

	DestructionEffectScale = FVector(3.5f, 3.5f, 3.5f);
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> ForceFieldAsset(TEXT("NiagaraSystem'/Game/GrimzaFX/Particles/NS_AuraHeal.NS_AuraHeal'"));
	if (ForceFieldAsset.Succeeded())
	{
		ForceFieldSystem = ForceFieldAsset.Object;

	}
	ForceFieldScale = FVector(1.0f, 1.0f, 1.0f);

	if (MoveComp)
	{
		MoveComp->Pattern = EArcadeMovement::BossSlowSweep; 
		MoveComp->Speed = 100.0f;
		MoveComp->Amplitude = 250.0f;
		MoveComp->Frequency = 0.3f;
	}

	if (BossCollision)
	{
		BossCollision->SetBoxExtent(FVector(500.0f, 400.0f, 400.0f)); 
	}
}
