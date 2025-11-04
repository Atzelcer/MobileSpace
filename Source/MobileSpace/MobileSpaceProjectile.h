// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundBase.h"
#include "MobileSpaceProjectile.generated.h"

class UProjectileMovementComponent;
class UStaticMeshComponent;

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Arma1,
	Arma2,
	Arma3,
	Arma4
};

UCLASS(config = Game)
class AMobileSpaceProjectile : public AActor
{
	GENERATED_BODY()

public:
	AMobileSpaceProjectile();

protected:
	UPROPERTY(VisibleAnywhere, Category = Projectile)
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(VisibleAnywhere, Category = Movement)
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere, Category = Effects)
	UParticleSystemComponent* ParticleProjectile;

	UPROPERTY(EditAnywhere, Category = Sound)
	USoundBase* FireSound;

	UPROPERTY(EditAnywhere, Category = Explosion)
	UParticleSystem* ExplosionParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	int32 EvolutionLevel; 

public:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse,
		const FHitResult& Hit);

	void PlayFireSound();
	void ApplyWeaponConfig(EWeaponType NewType);

	FORCEINLINE UStaticMeshComponent* GetProjectileMesh() const { return ProjectileMesh; }
	FORCEINLINE UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }
	FORCEINLINE int32 GetEvolutionLevel() const { return EvolutionLevel; }

	void SetWeaponAndEvolution(EWeaponType NewType, int32 NewLevel);

	UPROPERTY(EditAnywhere, Category = "Efectos|Explosion")
	float ExplosionScale;
};
