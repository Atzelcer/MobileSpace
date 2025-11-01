// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Sound/SoundWave.h"
#include "MegaCapsula.generated.h"

UCLASS()
class MOBILESPACE_API AMegaCapsula : public AActor
{
	GENERATED_BODY()

public:
	AMegaCapsula();

protected:
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* ParticleComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 TipoCapsula;

	UPROPERTY()
	USoundWave* SonidoRecoleccion;

	void AsignarParticulaAleatoria();
	void DestruirCapsula();
};
