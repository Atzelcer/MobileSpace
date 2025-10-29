// Fill out your copyright notice in the Description page of Project Settings.

// Copyright Epic Games, Inc.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ShipMultijugador.generated.h"

UCLASS()
class MOBILESPACE_API AShipMultijugador : public APawn
{
	GENERATED_BODY()

public:
	AShipMultijugador();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// === COMPONENTES ===
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
	class UStaticMeshComponent* ShipMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
	class UParticleSystemComponent* ParticleTrail;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	class USoundBase* FireSound;

	// === VARIABLES ===
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Estado")
	float VidaActual;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Estado")
	float VidaMaxima;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Movimiento")
	float VelocidadActual;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combate")
	int32 CantidadMisiles;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combate")
	int32 CantidadEscudos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combate")
	float LimiteDisparo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combate")
	bool bCanFire;

	// === FUNCIONES ===
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);

	void FireShot();

	void Server_FireShot_Implementation();

	UFUNCTION(Server, Reliable)
	void Server_FireShot();

	void ResetFire();

	void ActualizarHUD();

protected:
	FTimerHandle FireTimerHandle;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
