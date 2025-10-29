#include "MobileSpaceInstance.h"
#include "MultiplayerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Engine/World.h"
#include "GameMapsSettings.h"

void UMobileSpaceInstance::Init()
{
	Super::Init();
	UE_LOG(LogTemp, Warning, TEXT("GameInstance iniciado correctamente"));
}

void UMobileSpaceInstance::HostSala()
{
	UWorld* World = GetWorld();
	if (!World) return;

	if (!MultiplayerManager)
		MultiplayerManager = World->SpawnActor<AMultiplayerManager>();

	if (!MultiplayerManager) return;

	MultiplayerManager->CrearSala();

	FString IPLocal = ObtenerIPLocal();
	//UE_LOG(LogTemp, Warning, TEXT("Servidor listen activado en mapa actual. IP: %s"), *IPLocal);

	// Activa modo listen sin recargar el nivel
	const FString Command = FString::Printf(TEXT("open %s?listen"), *World->GetMapName());
	World->Exec(World, *Command);
}


void UMobileSpaceInstance::UnirseSala(const FString& CodigoIngresado)
{
	UWorld* World = GetWorld();
	if (!World) return;

	if (!MultiplayerManager)
	{
		TArray<AActor*> Managers;
		UGameplayStatics::GetAllActorsOfClass(World, AMultiplayerManager::StaticClass(), Managers);
		if (Managers.Num() > 0)
			MultiplayerManager = Cast<AMultiplayerManager>(Managers[0]);
	}

	if (!MultiplayerManager)
	{
		//UE_LOG(LogTemp, Warning, TEXT("No se encontró el Manager"));
		return;
	}

	FString HostIP = MultiplayerManager->IPHost;
	if (!VerificarConexionLAN(HostIP))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Cliente y host no están en la misma red LAN"));
		return;
	}

	MultiplayerManager->ValidarCodigoYUnirse(CodigoIngresado);
}

FString UMobileSpaceInstance::ObtenerIPLocal() const
{
	bool bCanBind = false;
	TSharedPtr<FInternetAddr> LocalAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, bCanBind);
	return LocalAddr.IsValid() ? LocalAddr->ToString(false) : TEXT("0.0.0.0");
}

bool UMobileSpaceInstance::VerificarConexionLAN(const FString& HostIP) const
{
	FString IPLocal = ObtenerIPLocal();
	TArray<FString> PartesLocal, PartesHost;
	IPLocal.ParseIntoArray(PartesLocal, TEXT("."));
	HostIP.ParseIntoArray(PartesHost, TEXT("."));

	if (PartesLocal.Num() < 3 || PartesHost.Num() < 3)
		return false;

	return (PartesLocal[0] == PartesHost[0] && PartesLocal[1] == PartesHost[1] && PartesLocal[2] == PartesHost[2]);
}
