// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundBase.h"
#include "Boss_Z.generated.h"


UCLASS()
class MOBILESPACE_API ABoss_Z : public ACharacter
{
	GENERATED_BODY()

public:
	ABoss_Z();

protected:
	virtual void BeginPlay() override;

	// Collision hit event
	UFUNCTION()
	virtual void OnBossHit(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

public:	
	virtual void Tick(float DeltaTime) override;


	UStaticMeshComponent* BossMesh;

	UBoxComponent* ShipCollision;

	void SpawnSequence();
	void DeathSequence();
	// Health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxHealth = 100.f;

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	float CurrentHealth;

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	bool bIsDead = false;

	// Partículas
	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* SpawnParticle;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* DeathParticle;

	
};

