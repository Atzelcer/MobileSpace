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
	ProjectileMesh->BodyInstance.SetCollisionProfileName("Projectile");
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AMobileSpaceProjectile::OnHit);
	// En MobileSpaceProjectile.cpp (constructor)
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ProjectileMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);// set up a notification for when this component hits something
	RootComponent = ProjectileMesh;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement0"));
	ProjectileMovement->UpdatedComponent = ProjectileMesh;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f; // No gravity


	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;

	ParticleProjectile = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	ParticleProjectile->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("ParticleSystem'/Game/TurretVFX/Sources/Particle/P_Pentagon.P_Pentagon'"));
	if (ParticleAsset.Succeeded())
	{
		ParticleProjectile->SetTemplate(ParticleAsset.Object);
		ParticleProjectile->SetRelativeScale3D(FVector(5.0, 5.0, 5.0));
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
	// Solo agrega impulso y reproduce el efecto de hit si golpea física
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 90.0f, GetActorLocation());
	}

	

	Destroy();
}