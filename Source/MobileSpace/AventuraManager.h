// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ship_X.h"
#include "Boss_Z.h"
#include "Ship_CazadorAlfa.h"
#include "Ship_CazadorDelta.h"
#include "Ship_CazadorVerde.h"
#include "Ship_CazadorRojo.h"
#include "Ship_CazadorAzul.h"
#include "Ship_CazadorOmega.h"
#include "DKraken_Boss_Z.h"
#include "DragonR_Boss_Z.h"
#include "DragonT_Boss_Z.h"
#include "MoveComponent.h" // ¡LIBRERÍA PARA LOS PATRONES CHISTOSOS!
#include "AventuraManager.generated.h"

UCLASS()
class MOBILESPACE_API AAventuraManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAventuraManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	UFUNCTION()
	void Nivel1();

	UFUNCTION()
	void Nivel2();

	UFUNCTION()
	void Nivel3();

	UFUNCTION()
	void Nivel4();

	UFUNCTION()
	void Nivel5();

	UFUNCTION()
	void Nivel6();

	UFUNCTION()
	void Nivel7();

	UFUNCTION()
	void CheckWaveComplete();

	UFUNCTION()
	void ControladorNiveles();

	UFUNCTION()
	void SetNivelActual(int32 NuevoNivel);

	void SiguienteNivel();

protected:
	UPROPERTY()
	int32 NivelActual;

	UPROPERTY()
	int32 CurrentWave;

	UPROPERTY()
	TArray<AShip_X*> ActiveShips;

	UPROPERTY()
	ABoss_Z* CurrentBoss;

	UPROPERTY()
	FTimerHandle WaveTimerHandle;

	UPROPERTY()
	FTimerHandle BossTimerHandle;
};
