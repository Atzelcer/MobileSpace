// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

	int32 NivelEnCurso;

	UFUNCTION()
	void EstablecerMaterial(UMaterialInstance* NuevoMaterial);

	UFUNCTION()
	void EscogerMaterialAleatorio();

	UFUNCTION()
	void ReproducirMusicaNivel(int32 Nivel);

	UFUNCTION()
	void ControladorNiveles();

	UFUNCTION()
	void SetNivelActual(int32 NuevoNivel);


	void SiguienteNivel();

	//Material Instance 


	UPROPERTY()
	class UMaterialInstance* MaterialGameMejor;

	bool Next_Oleada;

	int32 Current_Level;
	int32 Enemies_Oleadas;


	int32 TopeOleada;
	int32 OleadaActual;


private:
	int32 NivelActual;
};
