// Fill out your copyright notice in the Description page of Project Settings.

#include "DKraken_Boss_Z.h"
#include "UObject/ConstructorHelpers.h"
#include "Particles/ParticleSystem.h"

ADKraken_Boss_Z::ADKraken_Boss_Z()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BossMeshAsset(TEXT("StaticMesh'/Game/CF2Shuttle/Meshes/SM_CF2_Shuttle.SM_CF2_Shuttle'"));
	if (BossMeshAsset.Succeeded())
	{
		BossMesh->SetStaticMesh(BossMeshAsset.Object);
		BossMesh->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	
	}

	// Partícula de aparición
	static ConstructorHelpers::FObjectFinder<UParticleSystem> SpawnParticleAsset(
		TEXT("ParticleSystem'/Game/MFK/Particles_Tiny/SawSpark/Par_FW_Saw_01_Tiny.Par_FW_Saw_01_Tiny'"));
	if (SpawnParticleAsset.Succeeded())
	{
		SpawnParticle = SpawnParticleAsset.Object;
	}

	// Partícula de muerte
	static ConstructorHelpers::FObjectFinder<UParticleSystem> DeathParticleAsset(
		TEXT("ParticleSystem'/Game/MFK/Particles_Tiny/SawSpark/Par_FW_Saw_01_Tiny.Par_FW_Saw_01_Tiny'"));
	if (DeathParticleAsset.Succeeded())
	{
		DeathParticle = DeathParticleAsset.Object;
	}

	
}


