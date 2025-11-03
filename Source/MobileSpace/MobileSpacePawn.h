// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "NiagaraComponent.h"
#include "Sound/SoundWave.h"
#include "Particles/ParticleSystem.h"
#include "Components/CapsuleComponent.h"
#include "MobileSpaceProjectile.h"
#include "MobileSpacePawn.generated.h"

UCLASS(Blueprintable)
class AMobileSpacePawn : public APawn
{
	GENERATED_BODY()

public:
	AMobileSpacePawn();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	// ------------------------------------------------------------
	// Funciones principales
	// ------------------------------------------------------------
	void FireShot(FVector FireDirection);
	void ShotTimerExpired();

	void DispararMisil();
	void HacerDanio();
	void ActivarDanio();
	void EstablecerCapsula(int32 TipoCapsula);
	void InicializarPowerUpsHUD();

	// ------------------------------------------------------------
	// PowerUps (Escudo / Velocidad)
	// ------------------------------------------------------------
	void ActivarEscudo();
	void DesactivarEscudo();

	void ActivarVelocidad();
	void DesactivarVelocidad();

	// ------------------------------------------------------------
	// Bindings de entrada
	// ------------------------------------------------------------
	static const FName MoveForwardBinding;
	static const FName MoveRightBinding;
	static const FName FireForwardBinding;
	static const FName FireRightBinding;

	// ------------------------------------------------------------
	// Propiedades de movimiento
	// ------------------------------------------------------------
	UPROPERTY(EditAnywhere, Category = "Gameplay|Movimiento")
	float MoveSpeed;

	UPROPERTY(EditAnywhere, Category = "Gameplay|Movimiento")
	FVector GunOffset;

	UPROPERTY(EditAnywhere, Category = "Gameplay|Movimiento")
	FVector MisilOffset = FVector(85.471046f, 0.0f, 300.0f);

	UPROPERTY(EditAnywhere, Category = "Gameplay|Movimiento")
	float FireRate;

	UPROPERTY(EditAnywhere, Category = "Gameplay|Movimiento")
	FVector2D MovementMin = FVector2D(-1400.f, -3000.f);

	UPROPERTY(EditAnywhere, Category = "Gameplay|Movimiento")
	FVector2D MovementMax = FVector2D(1400.f, 3000.f);

	// ------------------------------------------------------------
	// Variables de estado de juego
	// ------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay|Estados")
	int32 CantVida;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay|Estados")
	int32 CantVelocidad;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay|Estados")
	int32 CantMissil;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay|Estados")
	int32 CantEscudo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay|Estados")
	int32 NumLifes;

	// ------------------------------------------------------------
	// Componentes principales
	// ------------------------------------------------------------
	UPROPERTY(VisibleAnywhere, Category = "Componentes|Colision", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, Category = "Componentes|Malla", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ShipMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Componentes|Efectos", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* ParticleTrail;

	UPROPERTY(VisibleAnywhere, Category = "Componentes|Efectos", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UNiagaraComponent* EscudoNiagara;

	// ------------------------------------------------------------
	// Recursos visuales y de audio
	// ------------------------------------------------------------
	UPROPERTY(EditDefaultsOnly, Category = "Efectos|Explosion")
	class UParticleSystem* ExplosionFX;

	UPROPERTY(EditDefaultsOnly, Category = "Efectos|Explosion")
	class USoundWave* ExplosionSound;

	UPROPERTY(EditDefaultsOnly, Category = "Efectos|Audio")
	class USoundWave* EscudoSound;

	UPROPERTY(EditDefaultsOnly, Category = "Efectos|Audio")
	class USoundWave* VelocidadSound;

	UPROPERTY(EditAnywhere, Category = "Efectos|Proyectil")
	TSubclassOf<AMobileSpaceProjectile> ProjectileClass;

	// ------------------------------------------------------------
	// Timers
	// ------------------------------------------------------------
	FTimerHandle TimerHandle_ShotTimerExpired;
	FTimerHandle TimerHandle_Escudo;
	FTimerHandle TimerHandle_Velocidad;
	FTimerHandle TimerHandle_hacerDanio;

	// ------------------------------------------------------------
	// Variables internas
	// ------------------------------------------------------------
	bool bCanFire;
	bool bInmuneEscudo;
	bool bHacerDanio;
	float VelocidadNormal;

	// ------------------------------------------------------------
	// Referencias externas
	// ------------------------------------------------------------
	UPROPERTY()
	class UNiagaraSystem* EscudoSystem;

	UPROPERTY()
	class UWidget_ON_GAME* HUDWidget;

	UPROPERTY()
	class AHUDmain* HUDRef;

};
