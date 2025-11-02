// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "MoveComponent.h"
#include "Sound/SoundBase.h"
#include "Ship_X.generated.h"


UCLASS()
class MOBILESPACE_API AShip_X : public AActor
{
	GENERATED_BODY()
	
public:	
	AShip_X();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* ShipMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* ShipCollision;

	UPROPERTY(VisibleAnywhere)
	UMoveComponent* MoveComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	USoundBase* DestructionSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	UParticleSystem* DestructionEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector DestructionEffectScale = FVector(1.0f, 1.0f, 1.0f);
	
	

protected:
	virtual void BeginPlay() override;



public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnShipHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void HandleDestruction();


protected:
	UPROPERTY(EditAnywhere, Category = "PowerUps")
	int32 ProbabilidadSpawnCapsula;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	class USoundBase* SonidoExplosion;

	void IntentarSpawnCapsula();

public:
	// Función virtual para que las clases hijas la sobreescriban
	virtual void DestruirNave();


};
