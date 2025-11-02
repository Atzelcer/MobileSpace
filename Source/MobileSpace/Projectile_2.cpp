// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile_2.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"
#include "Particles/ParticleSystem.h"

AProjectile_2::AProjectile_2()
{
	PrimaryActorTick.bCanEverTick = true;
	// Cambia solo el mesh y partícula, nada más
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(
		TEXT("/Game/TwinStick/Meshes/TwinStickProjectile.TwinStickProjectile")); // Cambia por el path que quieras
	if (MeshAsset.Succeeded())
	{
		ProjectileMesh->SetStaticMesh(MeshAsset.Object);
	}
	static ConstructorHelpers::FObjectFinder<UParticleSystem> TrailAsset(
		TEXT("ParticleSystem'/Game/MagicProjectilesVol2/Particles/Projectiles/P_Projectile_Orb06_Purple.P_Projectile_Orb06_Purple'")); // Cambia por el path que quieras
	if (TrailAsset.Succeeded())
	{
		ParticleProjectile->SetTemplate(TrailAsset.Object);
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> Fire_Sound(
		TEXT("SoundWave'/Game/LaserSounds/laser-312360.laser-312360'"));
	if (Fire_Sound.Succeeded())
		FireSound = Fire_Sound.Object;

	// Si quieres, ajusta velocidad, lifespan, etc. aquí
	ProjectileMovement->InitialSpeed = 5000.f;
	ProjectileMovement->MaxSpeed = 5000.f;
	InitialLifeSpan = 3.0f;
}
