// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ship_X.h"
#include "Boss_Z.h"
#include "ShipFactoryGeneral.h"
#include "MobEnums.h"
#include "AventuraManager.generated.h"

class AMobileSpacePawn;
class ACameraActor;
class UAudioComponent;
class AMegaPlataforma;

UCLASS()
class MOBILESPACE_API AAventuraManager : public AActor
{
    GENERATED_BODY()

public:
    AAventuraManager();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // === Clases para Spawn ===
    UPROPERTY(EditAnywhere, Category = "Spawn")
    TSubclassOf<AMobileSpacePawn> PawnClass;

    // === Control de niveles ===
    UFUNCTION() void Nivel1();
    UFUNCTION() void Nivel2();
    UFUNCTION() void Nivel3();
    UFUNCTION() void Nivel4();
    UFUNCTION() void Nivel5();
    UFUNCTION() void Nivel6();
    UFUNCTION() void Nivel7();
    UFUNCTION() void ControladorNiveles();
    UFUNCTION() void SetNivelActual(int32 NuevoNivel);

    void SiguienteNivel();

    // === Cámara fija ===
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    ACameraActor* FixedCamera;

    UFUNCTION(BlueprintCallable, Category = "Camera")
    void SetupFixedCamera();

protected:
    // === Estado del nivel ===
    UPROPERTY()
    int32 NivelActual;

    UPROPERTY()
    int32 CurrentWave;

    UPROPERTY()
    ABoss_Z* CurrentBoss;

    UPROPERTY()
    FTimerHandle WaveTimerHandle;

    UPROPERTY()
    FTimerHandle BossTimerHandle;

public:
    // === Fábrica de naves ===
    UPROPERTY()
    UShipFactoryGeneral* ShipFactory;

    FTimerHandle TimerHandle_IniciarNivel;

    // === Audio pantalla carga ===
    UPROPERTY()
    UAudioComponent* AudioComp_SonidoCarga;

    UPROPERTY()
    USoundWave* SonidoCarga;

    UFUNCTION()
    void ActivarEfectoSonidoPantallaCarga(bool bActivarSonido);

    UPROPERTY()
    FTimerHandle TimerHandle_DetenerSonido;

private:
    // === Oleadas normales ===
    int32 OleadasTotales;
    int32 CantidadPorOleada;
    int32 OleadaActual;
    TArray<ENaveTipo> TiposActuales;

    void GenerarOleada();
    void ComprobarOleadaGeneral();
    void SpawnPortalFinal();

    // === Oleadas de obstáculos ===
    void GenerarOleadaObstaculos();
    void ComprobarOleadaObstaculos();

public:
    int32 OleadaActualObstaculos = 0;
    int32 OleadasTotalesObstaculos = 3;
    int32 CantidadPorOleadaObstaculos = 6;

    FTimerHandle TimerHandle_ComprobarObstaculos;

    // === Movimiento jugador ===
    UFUNCTION() void TeletransportarJugador();
    UFUNCTION() void MoverJugador(float DeltaTime);

    void SpawnBoss();

    // === Generación de formaciones ===
    UFUNCTION()
    TArray<AShip_X*> GenerarEnjambre(
        ENaveTipo TipoNave,
        int32 Cantidad,
        FVector Centro,
        float Espaciado = 320.f,
        int32 Filas = 1
    );

    // === Audio oleadas ===
    UPROPERTY(EditDefaultsOnly, Category = "Audio")
    USoundBase* OleadaSound;

    FTimerHandle TimerHandle_Teletransporte;
    FTimerHandle TimerHandle_SpawnBoss;

    bool bJugadorMoviendose = false;
    float VelocidadTeletransporte = 550.f;

private:

    void ControlAtaqueAleatorio();


    UPROPERTY()
    AMegaPlataforma* PlataformaSpawn;

    FTimerHandle TimerHandle_ControlAtaques;
    float IntervaloControlAtaque = 0.25f;

    UPROPERTY(EditAnywhere, Category = "Gameplay|Movimiento")
    FVector2D MovementMin = FVector2D(-1400.f, -3000.f);

    UPROPERTY(EditAnywhere, Category = "Gameplay|Movimiento")
    FVector2D MovementMax = FVector2D(1400.f, 3000.f);

    UPROPERTY()
    TArray<AShip_X*> ActiveShips;

    // No usar UPROPERTY aquí (nested containers no permitidos por UHT)
    TMap<ENaveTipo, TArray<AShip_X*>> ShipsByType;
    TMap<EShipRole, TArray<AShip_X*>> ShipsByRole;


    // ============================================================
    // === FUNCIONES DE GESTIÓN DE NAVES ===
    // ============================================================

    // Añadir nave al registro general y a sus categorías
    void RegisterShip(AShip_X* Ship);

    // Remover nave del registro cuando muere o desaparece
    void UnregisterShip(AShip_X* Ship);

    // Obtener todas las naves de un tipo específico
    TArray<AShip_X*> GetShipsOfType(ENaveTipo Tipo);

    // Obtener naves con un rol específico
    TArray<AShip_X*> GetShipsWithRole(EShipRole Rol);

    // Activar/desactivar ataque en todas las naves de un tipo
    void EnableAttackForType(ENaveTipo Tipo, bool bEnable);

    // Activar/desactivar ataque en todas las naves de un rol
    void EnableAttackForRole(EShipRole Rol, bool bEnable);

    // Saber si ya no quedan naves activas (oleada terminada)
    bool IsWaveCleared() const;

};

