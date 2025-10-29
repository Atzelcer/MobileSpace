// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MobileSpaceProjectile.generated.h"

class UProjectileMovementComponent;
class UStaticMeshComponent;

UCLASS(config=Game)
class AMobileSpaceProjectile : public AActor
{
	GENERATED_BODY()

	/** Sphere collision component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ProjectileMesh;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	UParticleSystemComponent* ParticleProjectile;
	// Instancia (spawn temporal) para el hit effect, no adjunta por defecto
	UPROPERTY(EditDefaultsOnly, Category = Effects)
	UParticleSystem* HitParticleAsset;

	// Instancia (spawn temporal) para el muzzle effect (se usará desde el pawn)
	UPROPERTY(EditDefaultsOnly, Category = Effects)
	UParticleSystem* MuzzleParticleAsset;


public:
	AMobileSpaceProjectile();

	/** Function to handle the projectile hitting something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// Agrega aquí la definición de la función para spawn de muzzle effect
	void SpawnMuzzleEffect(const FVector& Location, const FRotator& Rotation);


	/** Returns ProjectileMesh subobject **/
	FORCEINLINE UStaticMeshComponent* GetProjectileMesh() const { return ProjectileMesh; }
	/** Returns ProjectileMovement subobject **/
	FORCEINLINE UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
};

