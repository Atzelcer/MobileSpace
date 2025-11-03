// Copyright Epic Games, Inc. All Rights Reserve

#include "MobileSpaceProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Ship_X.h"
#include "MegaObstaculo.h"

AMobileSpaceProjectile::AMobileSpaceProjectile() 
{

    ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh0"));
    RootComponent = ProjectileMesh;
    ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement0"));
    ProjectileMovement->UpdatedComponent = ProjectileMesh;
    ProjectileMovement->InitialSpeed = 5000.f;
    ProjectileMovement->MaxSpeed = 5000.f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->ProjectileGravityScale = 0.f;
    ProjectileMovement->bSweepCollision = true;
    InitialLifeSpan = 3.0f;

    ParticleProjectile = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
    ParticleProjectile->SetupAttachment(RootComponent);

    ProjectileMesh->OnComponentHit.AddDynamic(this, &AMobileSpaceProjectile::OnHit);
    
    FireSound = nullptr;
}

void AMobileSpaceProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    
    if (OtherActor && OtherActor != GetOwner() && OtherActor != this)
    {
        // Aplicar impulso físico si el objeto tiene física
        if (OtherComp && OtherComp->IsSimulatingPhysics())
        {
            OtherComp->AddImpulseAtLocation(GetVelocity() * 90.0f, GetActorLocation());
        }

        if (OtherActor->IsA(AShip_X::StaticClass()))
        {
            Cast<AShip_X>(OtherActor)->HandleDestruction();
        }


        if (OtherActor->IsA(AMegaObstaculo::StaticClass()))
        {
            Cast<AMegaObstaculo>(OtherActor)->DestruirObstaculo();
		}
        //else
        //{
        //    OtherActor->Destroy();
        //}

        Destroy();
		
    }


}


void AMobileSpaceProjectile::PlayFireSound()
{
    if (FireSound)
    {   
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
    }
}
   
    