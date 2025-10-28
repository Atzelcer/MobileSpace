// Fill out your copyright notice in the Description page of Project Settings.
#include "MultiplayerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "ShipMultijugador.h"
#include "TimerManager.h"
#include "GameFramework/PlayerController.h"
#include "WidgetSalaEspera.h"
#include "HUDmain.h"

AMultiplayerManager::AMultiplayerManager()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	bEsHost = false;
}

void AMultiplayerManager::BeginPlay()
{
	Super::BeginPlay();
}


void AMultiplayerManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMultiplayerManager::CrearSala(const FString& CodigoGenerado)
{
	UWorld* World = GetWorld();
	if (!World) return;

	bEsHost = true;
	CodigoSala = CodigoGenerado;
	HostController = UGameplayStatics::GetPlayerController(World, 0);

	UE_LOG(LogTemp, Warning, TEXT("Sala creada. Código: %s"), *CodigoSala);
}


void AMultiplayerManager::ValidarCodigoYUnirse(const FString& CodigoIngresado)
{
	if (CodigoIngresado == CodigoSala)
	{
		UE_LOG(LogTemp, Warning, TEXT("Código válido. Conectando cliente..."));
		ConfirmarConexionCliente();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Código inválido. No se puede unir."));
	}
}

void AMultiplayerManager::ConfirmarConexionCliente()
{
	UWorld* World = GetWorld();
	if (!World) return;

	ClienteController = UGameplayStatics::GetPlayerController(World, 0);
	if (!ClienteController) return;

	AHUDmain* HUD = Cast<AHUDmain>(ClienteController->GetHUD());
	if (!HUD) return;

	HUD->MostrarSalaEspera();

	if (HUD->WidgetSalaEsperaInstance)
	{
		HUD->WidgetSalaEsperaInstance->JugadorConectado(2);
	}

	World->GetTimerManager().SetTimer(TimerIniciarPartida, this, &AMultiplayerManager::IniciarPartida, 3.0f, false);
}


void AMultiplayerManager::IniciarPartida()
{
	UE_LOG(LogTemp, Warning, TEXT("Iniciando partida..."));
	SpawnearJugadores();
}



void AMultiplayerManager::SpawnearJugadores()
{
	UWorld* World = GetWorld();
	if (!World) return;

	const FVector SpawnHost(0, -800, 200);
	const FVector SpawnCliente(0, 800, 200);

	if (HostController && HostController->GetPawn() == nullptr)
	{
		APawn* HostShip = World->SpawnActor<AShipMultijugador>(AShipMultijugador::StaticClass(), SpawnHost, FRotator(0, 0, 0));
		if (HostShip) HostController->Possess(HostShip);
	}

	if (ClienteController && ClienteController->GetPawn() == nullptr)
	{
		APawn* ClientShip = World->SpawnActor<AShipMultijugador>(AShipMultijugador::StaticClass(), SpawnCliente, FRotator(0, 180, 0));
		if (ClientShip) ClienteController->Possess(ClientShip);
	}
}
