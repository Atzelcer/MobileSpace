// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MultiplayerManager.generated.h"

UCLASS()
class MOBILESPACE_API AMultiplayerManager : public AActor
{
	GENERATED_BODY()

public:
	AMultiplayerManager();

protected:
	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

	// === Lógica de conexión LAN ===
	UFUNCTION(BlueprintCallable, Category = "Multiplayer")
	void CrearSala(const FString& CodigoGenerado); // Host crea sala

	UFUNCTION(BlueprintCallable, Category = "Multiplayer")
	void ValidarCodigoYUnirse(const FString& CodigoIngresado); // Cliente intenta unirse

	UFUNCTION(BlueprintCallable, Category = "Multiplayer")
	void ConfirmarConexionCliente(); // Llamado cuando el cliente valida correctamente

	UFUNCTION(BlueprintCallable, Category = "Multiplayer")
	void SpawnearJugadores(); // Se ejecuta al pasar al juego

private:
	FString CodigoSala;
	bool bEsHost;

	UPROPERTY()
	APlayerController* HostController;

	UPROPERTY()
	APlayerController* ClienteController;

	FTimerHandle TimerIniciarPartida;
	void IniciarPartida();
};
