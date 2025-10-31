// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MegaPortal.generated.h"

class UCapsuleComponent;
class UNiagaraSystem;
class UNiagaraComponent;
class AAventuraManager;

UCLASS()
class MOBILESPACE_API AMegaPortal : public AActor
{
	GENERATED_BODY()

public:
	AMegaPortal();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

private:
	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* NiagaraPortalComponent;

	AAventuraManager* AventuraManagerPor;

	// Lista de posibles efectos Niagara
	UPROPERTY()
	TArray<UNiagaraSystem*> NiagaraSystems;

	void CargarNiagaraSystems();
	void EscogerEfectoAleatorio();
};
