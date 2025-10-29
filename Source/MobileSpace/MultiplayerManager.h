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

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void CrearSala();

	UFUNCTION(BlueprintCallable)
	void ValidarCodigoYUnirse(const FString& CodigoIngresado);

	UFUNCTION()
	void ConfirmarConexionCliente();

	UFUNCTION()
	void IniciarPartida();

	void SpawnearJugadores();

	void MostrarPantallaCarga();

	UPROPERTY(Replicated, VisibleAnywhere)
	FString CodigoSala;

	UPROPERTY(Replicated, VisibleAnywhere)
	FString IPHost;

	UPROPERTY()
	APlayerController* HostController;

	UPROPERTY()
	APlayerController* ClienteController;

	UPROPERTY()
	FTimerHandle TimerIniciarPartida;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
