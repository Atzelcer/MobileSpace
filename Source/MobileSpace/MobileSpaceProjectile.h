// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundBase.h"
#include "MobileSpaceProjectile.generated.h"

class UProjectileMovementComponent;
class UStaticMeshComponent;

UCLASS(config = Game)
class AMobileSpaceProjectile : public AActor
{
	GENERATED_BODY()

protected:
    UPROPERTY(VisibleAnywhere, Category = Projectile)
    UStaticMeshComponent* ProjectileMesh;

    UPROPERTY(VisibleAnywhere, Category = Movement)
    UProjectileMovementComponent* ProjectileMovement;

    UPROPERTY(VisibleAnywhere, Category = Effects)
    UParticleSystemComponent* ParticleProjectile;

	UPROPERTY(EditAnywhere, Category = Sound)
	USoundBase* FireSound;

public:
    AMobileSpaceProjectile();

    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, FVector NormalImpulse,
        const FHitResult& Hit);

    void PlayFireSound();

public:
    FORCEINLINE UStaticMeshComponent* GetProjectileMesh() const { return ProjectileMesh; }
    FORCEINLINE UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
};
	