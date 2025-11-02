// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MegaPortal.generated.h"

UCLASS()
class MOBILESPACE_API AMegaPortal : public AActor
{
	GENERATED_BODY()

public:
	AMegaPortal();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

private:
	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere)
	class UParticleSystemComponent* ParticlePortalComponent;

	UPROPERTY()
	TArray<UParticleSystem*> ParticleSystems;

	UPROPERTY()
	class AAventuraManager* AventuraManagerPor;

	void CargarParticleSystems();
	void EscogerEfectoAleatorio();

	void IrAlSiguienteNivelConTransicion();

	FTimerHandle TimerHandle_SiguienteNivel;

};
