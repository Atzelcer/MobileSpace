// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectileZero.generated.h"

UCLASS()
class MOBILESPACE_API AProjectileZero : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileZero();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* CollisionBox;

	// Configuración de daño
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float Damage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float ProjectileSpeed = 800.0f;

	// Efectos
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	UParticleSystem* HitEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	FVector HitEffectScale = FVector(1.0f, 1.0f, 1.0f);

	// Función de colisión
	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Función virtual para manejar daño - las hijas pueden sobreescribir
	virtual void DealDamageToPlayer(AActor* PlayerActor);

	// Función virtual para efectos de destrucción
	virtual void PlayDestructionEffects();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Función para destruir el proyectil
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	virtual void DestroyProjectile();

};
