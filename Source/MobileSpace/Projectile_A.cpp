// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile_A.h"
#include "Particles/ParticleSystemComponent.h"


AProjectile_A::AProjectile_A()
{
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ProjectileMeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	if (ProjectileMeshAsset.Succeeded())
	{
		ProjectileMesh->SetStaticMesh(ProjectileMeshAsset.Object);
	}

	P1 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	P1->SetupAttachment(RootComponent);
	

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystemAsset(TEXT("ParticleSystem'/Game/MagicProjectilesVol2/Particles/Projectiles/P_Projectile_Trail02_Red.P_Projectile_Trail02_Red'"));
	if (ParticleSystemAsset.Succeeded())
	{
		P1->SetTemplate(ParticleSystemAsset.Object);
	}
	
}

void AProjectile_A::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeSpan);
}

void AProjectile_A::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Mover hacia adelante
	FVector NewLocation = GetActorLocation() + GetActorForwardVector() * Speed * DeltaTime;
	SetActorLocation(NewLocation);
}
