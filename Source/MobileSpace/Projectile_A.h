// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileZero.h"
#include "Projectile_A.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESPACE_API AProjectile_A : public AProjectileZero
{
	GENERATED_BODY()

public:

	AProjectile_A();

	UParticleSystemComponent* P1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Speed = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lifetime")
	float LifeSpan = 5.0f;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
};
