// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile_C.h"
#include "Particles/ParticleSystemComponent.h"

AProjectile_C::AProjectile_C()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ProjectileMeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	if (ProjectileMeshAsset.Succeeded())
	{
		ProjectileMesh->SetStaticMesh(ProjectileMeshAsset.Object);
	}

	P3 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem_C"));
	P3->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystemAsset(TEXT("ParticleSystem'/Game/MagicProjectilesVol2/Particles/Projectiles/P_Projectile_Trail04_Orange.P_Projectile_Trail04_Orange'"));
	if (ParticleSystemAsset.Succeeded())
	{
		P3->SetTemplate(ParticleSystemAsset.Object);
	}
}

void AProjectile_C::BeginPlay()
{
	AActor::BeginPlay();
	
	SetLifeSpan(LifeSpan);
}

void AProjectile_C::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);
	FVector NewLocation = GetActorLocation() + GetActorForwardVector() * Speed * DeltaTime;
	SetActorLocation(NewLocation);
}
