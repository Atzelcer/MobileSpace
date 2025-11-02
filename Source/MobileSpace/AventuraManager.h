// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ship_X.h"
#include "Boss_Z.h"
#include "ShipFactoryGeneral.h"
#include "AventuraManager.generated.h"

class AMobileSpacePawn;

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

	UPROPERTY(EditAnywhere, Category = "Spawn")
	TSubclassOf<AMobileSpacePawn> PawnClass;

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
	void ControladorNiveles();

	UFUNCTION()
	void SetNivelActual(int32 NuevoNivel);

	void SiguienteNivel();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class ACameraActor* FixedCamera;

	UFUNCTION(BlueprintCallable, Category = "Camera")
	void SetupFixedCamera();

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

public:

	UPROPERTY()
	UShipFactoryGeneral* ShipFactory;


	FTimerHandle TimerHandle_IniciarNivel;

private:
	int32 OleadasTotales;
	int32 CantidadPorOleada;
	int32 OleadaActual;
	TArray<ENaveTipo> TiposActuales;

	void GenerarOleada();
	void ComprobarOleadaGeneral();
	void SpawnPortalFinal();

	void GenerarOleadaObstaculos();
	void ComprobarOleadaObstaculos();

private:
	int32 OleadaActualObstaculos = 0;
	int32 OleadasTotalesObstaculos = 3;
	int32 CantidadPorOleadaObstaculos = 6;

	FTimerHandle TimerHandle_ComprobarObstaculos;
};
