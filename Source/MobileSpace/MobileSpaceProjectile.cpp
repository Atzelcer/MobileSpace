// Copyright Epic Games, Inc. All Rights Reserve

#include "MobileSpaceProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/StaticMesh.h"
#include "Particles/ParticleSystemComponent.h"

AMobileSpaceProjectile::AMobileSpaceProjectile() 
{
	// Static reference to the mesh to use for the projectile
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ProjectileMeshAsset(TEXT("/Game/TwinStick/Meshes/TwinStickProjectile.TwinStickProjectile"));

	// Create mesh component for the projectile sphere
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh0"));
	ProjectileMesh->SetStaticMesh(ProjectileMeshAsset.Object);
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->SetVisibility(false);
	
	// Configuración de colisión mejorada
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ProjectileMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	ProjectileMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore); // No golpear al jugador
	
	// Vincular eventos de colisión
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AMobileSpaceProjectile::OnHit);
	
	RootComponent = ProjectileMesh;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement0"));
	ProjectileMovement->UpdatedComponent = ProjectileMesh;
	ProjectileMovement->InitialSpeed = 5000.f;
	ProjectileMovement->MaxSpeed = 5000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f; // No gravity
	ProjectileMovement->bSweepCollision = true; // Importante: Habilitar detección de colisión por barrido


	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;

	ParticleProjectile = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	ParticleProjectile->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("ParticleSystem'/Game/TurretVFX/Sources/Particle/P_EnergyBolt.P_EnergyBolt'"));
	if (ParticleAsset.Succeeded())
	{
		ParticleProjectile->SetTemplate(ParticleAsset.Object);
		ParticleProjectile->SetRelativeScale3D(FVector(2.0, 2.0, 2.0));
	}

	ParticleHit = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleHit"));
	ParticleHit->SetupAttachment(RootComponent);
	ParticleHit->bAutoActivate = true;
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleHitAsset(TEXT("ParticleSystem'/Game/MagicProjectilesVol2/Particles/Projectiles/P_Projectile_Trail03_Purple.P_Projectile_Trail03_Purple'"));
	if (ParticleHitAsset.Succeeded())
	{
		ParticleHit->SetTemplate(ParticleHitAsset.Object);
	}

}

void AMobileSpaceProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Debug para ver qué está golpeando
	if (GEngine && OtherActor)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, 
			FString::Printf(TEXT("Projectile hit: %s"), *OtherActor->GetName()));
	}

	// No golpear al jugador que disparó
	if (OtherActor && OtherActor != GetOwner() && OtherActor != this)
	{
		// Si golpea algo que simula física, agregar impulso
		if (OtherComp && OtherComp->IsSimulatingPhysics())
		{
			OtherComp->AddImpulseAtLocation(GetVelocity() * 90.0f, GetActorLocation());
		}

		// ¡DESTRUIR AMBOS! 😂
		// Destruir la cosa que golpeamos (nave enemiga, etc.)
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange, 
				FString::Printf(TEXT("Destroying target: %s"), *OtherActor->GetName()));
		}
		OtherActor->Destroy();

		// Destruir el proyectil también
		//if (GEngine)
		//{
		//	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("Projectile destroyed on impact!"));
		//}
		Destroy();
	}
}