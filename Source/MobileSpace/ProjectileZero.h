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
	AProjectileZero();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* CollisionBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float Damage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float ProjectileSpeed = 800.0f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	FVector HitEffectScale = FVector(1.0f, 1.0f, 1.0f);

	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	

protected:
	virtual void BeginPlay() override;

	virtual void DealDamageToPlayer(AActor* PlayerActor);

	virtual void PlayDestructionEffects();

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	virtual void DestroyProjectile();

};
