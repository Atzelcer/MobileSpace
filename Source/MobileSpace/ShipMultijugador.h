// Fill out your copyright notice in the Description page of Project Settings.

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

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	class UStaticMeshComponent* ShipMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VFX")
	class UParticleSystemComponent* ParticleTrail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UWidgetOnGameMulti> WidgetMultiClass;

	UPROPERTY()
	class UWidgetOnGameMulti* WidgetMultiInstance;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
	float VidaActual;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float VidaMaxima;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
	float VelocidadActual;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 CantidadMisiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 CantidadEscudos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float LimiteDisparo;

	void ActualizarHUD();
	void MoveForward(float Value);
	void MoveRight(float Value);
	void FireShot();

	void Server_FireShot_Implementation();

	UFUNCTION(Server, Reliable)
	void Server_FireShot();

private:
	bool bCanFire;
	FTimerHandle FireTimerHandle;
	void ResetFire();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
