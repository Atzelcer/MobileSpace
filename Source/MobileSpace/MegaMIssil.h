// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundWave.h"
#include "MegaMIssil.generated.h"

UCLASS()
class MOBILESPACE_API AMegaMIssil : public AActor
{
	GENERATED_BODY()

public:
	AMegaMIssil();

protected:
	virtual void BeginPlay() override;

	// --- Componentes ---
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UParticleSystemComponent* TrailParticle;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UProjectileMovementComponent* ProjectileMovement;

	// --- Efectos ---
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* ExplosionParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundWave* LaunchSound;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundWave* ExplosionSound;

	// --- Gameplay ---
	UPROPERTY(EditAnywhere, Category = "Gameplay")
	float ExplosionDelay;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	float ExplosionRadius;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	float DamageAmount;


	// NUEVO
	UPROPERTY(EditAnywhere, Category = "Movimiento")
	FVector Destino;

	UPROPERTY(EditAnywhere, Category = "Movimiento")
	float VelocidadVuelo;

	FTimerHandle ExplosionTimerHandle;

	UFUNCTION()
	void TriggerExplosion();

public:
	virtual void Tick(float DeltaTime) override;
	void PrepararExplosion();
};
