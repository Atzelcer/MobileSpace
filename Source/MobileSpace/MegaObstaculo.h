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
	float Velocidad = 500.f;

	void MoverObstaculo(float DeltaTime);
	void VerificarDestruccion();

};
