// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss_1.h"
#include "UObject/ConstructorHelpers.h"

ABoss_1::ABoss_1()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BossMeshAsset(
		TEXT("StaticMesh'/Game/StarSparrow/Meshes/Examples/SM_StarSparrow11.SM_StarSparrow11'"));

	if (BossMeshAsset.Succeeded() && BossMesh)
	{
		BossMesh->SetStaticMesh(BossMeshAsset.Object);
		BossMesh->SetRelativeScale3D(FVector(1.2f, 1.2f, 1.2f));
		
	}
	
	static ConstructorHelpers::FObjectFinder<USoundBase> AppearanceSoundAsset(
		TEXT("/Game/BOSS_SOUNDS/EveryTime.EveryTime"));

	if (AppearanceSoundAsset.Succeeded())
	{
		AppearanceSound = AppearanceSoundAsset.Object;
	}
	
	EntranceHeight = 800.0f;
	EntranceSpeed = 200.0f;
	EntranceDuration = 4.0f;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> AppearanceEffectAsset(
		TEXT("ParticleSystem'/Game/MFK/Particles/Expanders/Par_ExpFire_01_Rain.Par_ExpFire_01_Rain'"));
	
	if (AppearanceEffectAsset.Succeeded())
	{
		AppearanceEffect = AppearanceEffectAsset.Object;
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

	BossHealth = 3000;
	CurrentHealth = BossHealth;
	FireRate = 1.8f;
	AttackPattern = EAtackPattern::Spread;

	if (MoveComp)
	{
		MoveComp->Speed = 120.0f;
		MoveComp->Amplitude = 300.0f;
		MoveComp->Frequency = 0.4f;
	}

	DestructionEffectScale = FVector(3.0f, 3.0f, 3.0f);
	
	static ConstructorHelpers::FObjectFinder<UParticleSystem> DestructionEffectAsset(
		TEXT("ParticleSystem'/Game/MFK/Particles/Expanders/Par_ExpFire_01_Rain.Par_ExpFire_01_Rain'"));
	
	if (DestructionEffectAsset.Succeeded())
	{
		DestructionEffect = DestructionEffectAsset.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<USoundBase> DestructionSoundAsset(
		TEXT("SoundWave'/Game/GoodFXLevelUp/SFX/Sound_Wave/A_GFXLU_cinematic-hit-deep-and-dirty-thud-with-pitch-time-wobble.A_GFXLU_cinematic-hit-deep-and-dirty-thud-with-pitch-time-wobble'"));
	
	if (DestructionSoundAsset.Succeeded())
	{
		DestructionSound = DestructionSoundAsset.Object;
	}
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> ForceFieldAsset(TEXT("NiagaraSystem'/Game/GrimzaFX/Particles/NS_AuraHeal.NS_AuraHeal'"));
	if (ForceFieldAsset.Succeeded())
	{
		ForceFieldSystem = ForceFieldAsset.Object;

	}
	ForceFieldScale = FVector(1.3f, 1.3f, 1.3f); 
	if (BossCollision)
	{
		BossCollision->SetBoxExtent(FVector(500.0f, 400.0f, 400.0f));
	}
}

