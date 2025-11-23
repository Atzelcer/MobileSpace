// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ship_X.h"
#include "ShipFactoryGeneral.h"
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

    UPROPERTY(EditAnywhere, Category = "Spawn")
    TSubclassOf<AMobileSpacePawn> PawnClass;

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

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    ACameraActor* FixedCamera;

    UFUNCTION(BlueprintCallable, Category = "Camera")
    void SetupFixedCamera();

protected:
    UPROPERTY()
    int32 NivelActual;

    UPROPERTY()
    int32 CurrentWave;



    UPROPERTY()
    FTimerHandle WaveTimerHandle;

    UPROPERTY()
    FTimerHandle BossTimerHandle;

public:
    UPROPERTY()
    UShipFactoryGeneral* ShipFactory;

    FTimerHandle TimerHandle_IniciarNivel;

    UPROPERTY()
    UAudioComponent* AudioComp_SonidoCarga;

    UPROPERTY()
    USoundWave* SonidoCarga;

    UFUNCTION()
    void ActivarEfectoSonidoPantallaCarga(bool bActivarSonido);

    UPROPERTY()
    FTimerHandle TimerHandle_DetenerSonido;

private:
    int32 OleadasTotales;
    int32 CantidadPorOleada;
    int32 OleadaActual;
    TArray<ENaveTipo> TiposActuales;

    void GenerarOleada();
    void ComprobarOleadaGeneral();
    void SpawnPortalFinal();
    
    bool bOleadasCompletadas = false;
    bool bJefeActivo = false;
    bool bJefeEliminado = false;
    
    void SpawnJefeDelNivel();
    void ComprobarJefe();
    int32 GetTipoJefeParaNivel(int32 Nivel);
    
    // Función de debug para verificar estado manualmente
    UFUNCTION(BlueprintCallable, Category = "Debug")
    void DebugEstadoJefe();
    
    // Función para forzar eliminación de jefe manualmente
    UFUNCTION(BlueprintCallable, Category = "Debug") 
    void ForzarEliminacionJefe();

    void GenerarOleadaObstaculos();
    void ComprobarOleadaObstaculos();

public:
    int32 OleadaActualObstaculos = 0;
    int32 OleadasTotalesObstaculos = 3;
    int32 CantidadPorOleadaObstaculos = 6;

    FTimerHandle TimerHandle_ComprobarObstaculos;

    UFUNCTION() void TeletransportarJugador();
    UFUNCTION() void MoverJugador(float DeltaTime);


    //USoundBase* OleadaSound;

    FTimerHandle TimerHandle_Teletransporte;
    FTimerHandle TimerHandle_SpawnBoss;

    bool bJugadorMoviendose = false;
    float VelocidadTeletransporte = 550.f;

private:



    UPROPERTY()
    AMegaPlataforma* PlataformaSpawn;

    FTimerHandle TimerHandle_ControlAtaques;
    float IntervaloControlAtaque = 0.25f;

    UPROPERTY(EditAnywhere, Category = "Gameplay|Movimiento")
    FVector2D MovementMin = FVector2D(-1400.f, -3000.f);

    UPROPERTY(EditAnywhere, Category = "Gameplay|Movimiento")
    FVector2D MovementMax = FVector2D(1400.f, 3000.f);

  
   

};

