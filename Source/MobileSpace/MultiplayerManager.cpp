// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiplayerManager.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "HUDmain.h"
#include "WidgetSalaEspera.h"
#include "ShipMultijugador.h"
#include "Sockets.h"
#include "SocketSubsystem.h"

AMultiplayerManager::AMultiplayerManager()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void AMultiplayerManager::BeginPlay()
{
	Super::BeginPlay();
}

void AMultiplayerManager::CrearSala()
{
	CodigoSala = FString::FromInt(FMath::RandRange(1000, 9999));
	bool bCanBind = false;
	TSharedPtr<FInternetAddr> LocalAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, bCanBind);
	IPHost = LocalAddr.IsValid() ? LocalAddr->ToString(false) : TEXT("0.0.0.0");

	UWorld* World = GetWorld();
	if (!World) return;

	HostController = UGameplayStatics::GetPlayerController(World, 0);
	if (HostController)
	{
		AHUDmain* HUD = Cast<AHUDmain>(HostController->GetHUD());
		HUD->MostrarSalaEspera();
		if (HUD && HUD->WidgetSalaEsperaInstance)
			HUD->WidgetSalaEsperaInstance->ActualizarCodigo(CodigoSala);
	}
}

void AMultiplayerManager::ValidarCodigoYUnirse(const FString& CodigoIngresado)
{
	if (CodigoIngresado == CodigoSala)
	{
		ConfirmarConexionCliente();
	}
}

void AMultiplayerManager::ConfirmarConexionCliente()
{
	UWorld* World = GetWorld();
	if (!World) return;

	ClienteController = UGameplayStatics::GetPlayerController(World, 0);
	if (!ClienteController) return;

	AHUDmain* HUD = Cast<AHUDmain>(ClienteController->GetHUD());
	if (HUD)
	{
		HUD->MostrarSalaEspera();
		if (HUD->WidgetSalaEsperaInstance)
			HUD->WidgetSalaEsperaInstance->JugadorConectado(2);
	}


	World->GetTimerManager().SetTimer(TimerIniciarPartida, this, &AMultiplayerManager::MostrarPantallaCarga, 3.0f, false);
}

void AMultiplayerManager::MostrarPantallaCarga()
{
	UWorld* World = GetWorld();
	if (!World) return;

	APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
	if (!PC) return;

	AHUDmain* HUD = Cast<AHUDmain>(PC->GetHUD());
	if (!HUD) return;

	HUD->MostrarPantallaCargaMulti();

	World->GetTimerManager().SetTimer(TimerIniciarPartida, this, &AMultiplayerManager::IniciarPartida, 5.0f, false);

}

void AMultiplayerManager::IniciarPartida()
{
	//UE_LOG(LogTemp, Warning, TEXT("Iniciando partida..."));
	SpawnearJugadores();
}


void AMultiplayerManager::SpawnearJugadores()
{
	UWorld* World = GetWorld();
	if (!World) return;

	const FVector CentroMapa(0, 0, 200);
	const FVector SpawnHost(0, -800, 200);
	const FVector SpawnCliente(0, 800, 200);

	if (HostController && !HostController->GetPawn())
	{
		FRotator RotacionHost = (CentroMapa - SpawnHost).Rotation();
		AShipMultijugador* HostShip = World->SpawnActor<AShipMultijugador>(
			AShipMultijugador::StaticClass(), SpawnHost, RotacionHost);
		if (HostShip) HostController->Possess(HostShip);
	}

	if (ClienteController && !ClienteController->GetPawn())
	{
		FRotator RotacionCliente = (CentroMapa - SpawnCliente).Rotation();
		AShipMultijugador* ClientShip = World->SpawnActor<AShipMultijugador>(
			AShipMultijugador::StaticClass(), SpawnCliente, RotacionCliente);
		if (ClientShip) ClienteController->Possess(ClientShip);
	}

	if (HostController)
	{
		AHUDmain* HUDHost = Cast<AHUDmain>(HostController->GetHUD());
		if (HUDHost)
		{
			//HUDHost->OcultarTodo();
			HUDHost->MostrarOnGameMulti();
		}
	}

	if (ClienteController)
	{
		AHUDmain* HUDClient = Cast<AHUDmain>(ClienteController->GetHUD());
		if (HUDClient)
		{
			//HUDClient->OcultarTodo();
			HUDClient->MostrarOnGameMulti();
		}
	}
}


void AMultiplayerManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMultiplayerManager, CodigoSala);
	DOREPLIFETIME(AMultiplayerManager, IPHost);
}
