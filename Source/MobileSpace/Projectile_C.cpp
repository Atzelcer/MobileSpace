// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile_C.h"
#include "Particles/ParticleSystemComponent.h"

AProjectile_C::AProjectile_C()
{
	PrimaryActorTick.bCanEverTick = true;

	// Configurar malla específica para Projectile_C
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
	
	Damage = 25.0f; 
	ProjectileSpeed = 0.0f;  

}

void AProjectile_C::BeginPlay()
{
	Super::BeginPlay(); 
	SetLifeSpan(LifeSpan);
}

void AProjectile_C::Tick(float DeltaTime)
{
	
	FVector NewLocation = GetActorLocation() + GetActorForwardVector() * Speed * DeltaTime;
	SetActorLocation(NewLocation);
	
	if (NewLocation.X < -2000.0f || NewLocation.X > 2000.0f || 
		FMath::Abs(NewLocation.Y) > 3000.0f)
	{
		Destroy();
	}
}
