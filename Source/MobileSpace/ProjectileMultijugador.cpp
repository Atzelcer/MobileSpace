// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectileMultijugador.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "UObject/ConstructorHelpers.h"

AProjectileMultijugador::AProjectileMultijugador()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true; // Importante para red

	// --- Malla del proyectil ---
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/TwinStick/Meshes/TwinStickProjectile.TwinStickProjectile"));
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	RootComponent = ProjectileMesh;

	if (MeshAsset.Succeeded())
		ProjectileMesh->SetStaticMesh(MeshAsset.Object);

	ProjectileMesh->SetCollisionProfileName("Projectile");
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectileMultijugador::OnHit);
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ProjectileMesh->SetCollisionResponseToAllChannels(ECR_Overlap);

	// --- Movimiento ---
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = ProjectileMesh;
	ProjectileMovement->InitialSpeed = 3500.f;
	ProjectileMovement->MaxSpeed = 3500.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0.f;

	InitialLifeSpan = 2.5f;

	// --- Partículas ---
	ParticleTrail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TrailFX"));
	ParticleTrail->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UParticleSystem> TrailFX(TEXT("ParticleSystem'/Game/MagicProjectilesVol2/Particles/Projectiles/P_Projectile_Trail03_Purple.P_Projectile_Trail03_Purple'"));
	if (TrailFX.Succeeded())
	{
		ParticleTrail->SetTemplate(TrailFX.Object);
		ParticleTrail->bAutoActivate = true;
	}

	ParticleImpact = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ImpactFX"));
	ParticleImpact->SetupAttachment(RootComponent);
	ParticleImpact->bAutoActivate = false;
}

void AProjectileMultijugador::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectileMultijugador::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectileMultijugador::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (HasAuthority()) // Solo el servidor destruye
	{
		Destroy();
	}
}
