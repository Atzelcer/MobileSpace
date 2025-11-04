// Fill out your copyright notice in the Description page of Project Settings.

#include "MegaMIssil.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "MobileSpacePawn.h"
#include "MegaCapsula.h"
#include "Components/SphereComponent.h"
#include "Ship_X.h"
#include "MegaObstaculo.h"

AMegaMIssil::AMegaMIssil()
{
	PrimaryActorTick.bCanEverTick = true;

	// --- Colisión ---
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;
	CollisionComponent->InitSphereRadius(30.0f);
	CollisionComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));

	// --- Partícula de cola ---
	TrailParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TrailParticle"));
	TrailParticle->SetupAttachment(RootComponent);
	TrailParticle->SetTemplate(LoadObject<UParticleSystem>(nullptr, TEXT("/Game/VFXSeries1/Particles/Tails/P_RocketTypeB.P_RocketTypeB")));
	TrailParticle->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	TrailParticle->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));

	// --- Movimiento del proyectil ---
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = CollisionComponent;
	ProjectileMovement->InitialSpeed = 4000.0f;
	ProjectileMovement->MaxSpeed = 4000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.0f;

	// --- Carga de recursos ---
	ExplosionParticle = LoadObject<UParticleSystem>(nullptr, TEXT("/Game/TurretVFX/Sources/Particle/Hit/P_FireBall_Hit.P_FireBall_Hit"));
	LaunchSound = LoadObject<USoundWave>(nullptr, TEXT("/Game/MFK/Sounds/Launches/A_Launch_06.A_Launch_06"));
	ExplosionSound = LoadObject<USoundWave>(nullptr, TEXT("/Game/MFK/Sounds/Explode/A_Explode_18.A_Explode_18"));

	// --- Parámetros ---
	ExplosionRadius = 1000.0f;
	DamageAmount = 99999.0f;
	InitialLifeSpan = 5.0f;

	Destino = FVector(85.471046f, 0.0f, 300.0f);
	VelocidadVuelo = 2500.0f;
}

void AMegaMIssil::BeginPlay()
{
	Super::BeginPlay();

	if (LaunchSound)
		UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());
}

void AMegaMIssil::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!GetWorld()) return;

	FVector PosActual = GetActorLocation();
	FVector Direccion = (Destino - PosActual).GetSafeNormal();
	float Distancia = FVector::Dist(PosActual, Destino);

	SetActorLocation(PosActual + Direccion * VelocidadVuelo * DeltaTime, true);

	if (Distancia <= 50.0f)
	{
		PrepararExplosion();
	}
}

void AMegaMIssil::PrepararExplosion()
{
	SetActorTickEnabled(false);

	// --- Buscar todas las naves y obstáculos activos ---
	TArray<AActor*> AllActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), AllActors);

	for (AActor* Actor : AllActors)
	{
		if (!Actor || Actor == this) continue;

		// Si es una nave o hija de AShip_X ? HandleDestruction()
		if (Actor->IsA(AShip_X::StaticClass()))
		{
			AShip_X* Ship = Cast<AShip_X>(Actor);
			if (Ship)
			{
				Ship->HandleDestruction();
				continue;
			}
		}

		// Si es un obstáculo ? DestruirObstaculo()
		if (Actor->IsA(AMegaObstaculo::StaticClass()))
		{
			AMegaObstaculo* Ob = Cast<AMegaObstaculo>(Actor);
			if (Ob)
			{
				Ob->DestruirObstaculo();
				continue;
			}
		}
	}

	// Después de destruir, generar efectos
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AMegaMIssil::TriggerExplosion, 0.15f, false);
}

void AMegaMIssil::TriggerExplosion()
{
	if (!GetWorld()) return;

	if (ExplosionParticle)
	{
		UParticleSystemComponent* ExplosionPSC = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(), ExplosionParticle, GetActorTransform(), true);
		if (ExplosionPSC)
			ExplosionPSC->SetWorldScale3D(FVector(35.f, 35.f, 35.f));
	}

	if (ExplosionSound)
		UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());

	Destroy();
}
