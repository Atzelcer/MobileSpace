// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile_B.h"
#include "Particles/ParticleSystemComponent.h"

AProjectile_B::AProjectile_B()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ProjectileMeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	if (ProjectileMeshAsset.Succeeded())
	{
		ProjectileMesh->SetStaticMesh(ProjectileMeshAsset.Object);
		
	}

	P2 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem_B"));
	P2->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystemAsset(TEXT("ParticleSystem'/Game/MagicProjectilesVol2/Particles/Projectiles/P_Projectile_Trail01_Purple.P_Projectile_Trail01_Purple'"));
	if (ParticleSystemAsset.Succeeded())
	{
		P2->SetTemplate(ParticleSystemAsset.Object);
	}
	
	Damage = 20.0f; 
	ProjectileSpeed = 0.0f; 
	
}

void AProjectile_B::BeginPlay()
{
	AProjectileZero::BeginPlay();
	SetLifeSpan(LifeSpan);
}

void AProjectile_B::Tick(float DeltaTime)
{
	
	FVector NewLocation = GetActorLocation() + GetActorForwardVector() * Speed * DeltaTime;
	SetActorLocation(NewLocation);
	
	if (NewLocation.X < -2000.0f || NewLocation.X > 2000.0f || 
		FMath::Abs(NewLocation.Y) > 3000.0f)
	{
		Destroy();
	}
}
