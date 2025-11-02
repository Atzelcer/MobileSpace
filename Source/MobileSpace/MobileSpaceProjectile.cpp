// Copyright Epic Games, Inc. All Rights Reserve

#include "MobileSpaceProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Ship_X.h"

AMobileSpaceProjectile::AMobileSpaceProjectile() 
{
	// Static reference to the mesh to use for the projectile
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ProjectileMeshAsset(TEXT("/Game/TwinStick/Meshes/TwinStickProjectile.TwinStickProjectile"));

	// Create mesh component for the projectile sphere
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh0"));
	ProjectileMesh->SetStaticMesh(ProjectileMeshAsset.Object);
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->SetVisibility(false);
	
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ProjectileMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	ProjectileMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore); 
	
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AMobileSpaceProjectile::OnHit);
	
	RootComponent = ProjectileMesh;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement0"));
	ProjectileMovement->UpdatedComponent = ProjectileMesh;
	ProjectileMovement->InitialSpeed = 5000.f;
	ProjectileMovement->MaxSpeed = 5000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f; 
	ProjectileMovement->bSweepCollision = true; 


	InitialLifeSpan = 3.0f;

	ParticleProjectile = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	ParticleProjectile->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("ParticleSystem'/Game/MagicProjectilesVol2/Particles/Projectiles/P_Projectile_Trail05_Red.P_Projectile_Trail05_Red'"));
	if (ParticleAsset.Succeeded())
	{
		ParticleProjectile->SetTemplate(ParticleAsset.Object);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleHitAsset(TEXT("ParticleSystem'/Game/MagicProjectilesVol2/Particles/Hits/P_Hit_Trail05_Red.P_Hit_Trail05_Red'"));
	if (ParticleHitAsset.Succeeded()) {
		HitParticleAsset = ParticleHitAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleMuzzleAsset(TEXT("ParticleSystem'/Game/MagicProjectilesVol2/Particles/Muzzles/P_Muzzle_Trail05_Red.P_Muzzle_Trail05_Red'"));
	if (ParticleMuzzleAsset.Succeeded()) {
		MuzzleParticleAsset = ParticleMuzzleAsset.Object;
	}
}

void AMobileSpaceProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (HitParticleAsset) {
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticleAsset, Hit.Location, Hit.Normal.Rotation());
	}

	if (OtherActor && OtherActor != GetOwner() && OtherActor != this) {
		if (OtherComp && OtherComp->IsSimulatingPhysics())
			OtherComp->AddImpulseAtLocation(GetVelocity() * 90.0f, GetActorLocation());

		if (OtherActor->IsA(AShip_X::StaticClass())) {
			Cast<AShip_X>(OtherActor)->DestruirNave();
		}
		else {
			OtherActor->Destroy(); 
		}
		Destroy();
	}
}

void AMobileSpaceProjectile::SpawnMuzzleEffect(const FVector& Location, const FRotator& Rotation)
{
	if (MuzzleParticleAsset)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleParticleAsset, Location, Rotation);
	}
}
