// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Sound/SoundWave.h"
#include "MegaObstaculo.generated.h"

UCLASS()
class MOBILESPACE_API AMegaObstaculo : public AActor
{
	GENERATED_BODY()

public:
	AMegaObstaculo();

protected:
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* CollisionComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 TipoObstaculo;

	UPROPERTY()
	USoundWave* SonidoDestruccion;

	void AsignarMallaAleatoria();
	void DestruirObstaculo();

	UPROPERTY(EditAnywhere, Category = "Movimiento")
	float VelocidadMovimiento = 1200.f;

	// ===== SISTEMA DE MOVIMIENTO DIAGONAL =====
	UPROPERTY(EditAnywhere, Category = "Movimiento")
	FVector DireccionMovimiento = FVector(1.0f, 0.0f, 0.0f); // Por defecto hacia la izquierda

	UPROPERTY(EditAnywhere, Category = "Movimiento")
	bool bMovimientoDiagonal = false;

	void MoverObstaculo(float DeltaTime);
	void VerificarDestruccion();

	void SetVelocidadMovimiento(float NuevaVelocidad);


};
