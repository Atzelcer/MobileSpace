#include "AventuraManager.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Ship_CazadorAlfa.h"
#include "Ship_CazadorDelta.h"
#include "Ship_CazadorVerde.h"
#include "Ship_CazadorRojo.h"
#include "Ship_CazadorAzul.h"
#include "Ship_CazadorOmega.h"
//#include "DKraken_Boss_Z.h"
//#include "DragonR_Boss_Z.h"
//#include "DragonT_Boss_Z.h"
#include "MobileSpacePawn.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "MegaPortal.h"
#include "MegaObstaculo.h"
//#include "Boss_Z.h"
#include "HUDmain.h"
#include "MegaPlataforma.h"

AAventuraManager::AAventuraManager()
{
	PrimaryActorTick.bCanEverTick = true;

	NivelActual = 1;
	CurrentWave = 1;
	//CurrentBoss = nullptr;

	AudioComp_SonidoCarga = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp_SonidoCarga"));
	AudioComp_SonidoCarga->SetupAttachment(RootComponent);
	AudioComp_SonidoCarga->bAutoActivate = false;

	SonidoCarga = LoadObject<USoundWave>(nullptr,
		TEXT("SoundWave'/Game/AuroraSoundTrack/Wav/Interstellar_Drift.Interstellar_Drift'"));

	static ConstructorHelpers::FObjectFinder<USoundBase> OleadaSoundAsset(
		TEXT("SoundWave'/Game/Musica_D/EpicToonSFX/BonusChargeWhooshAudio/CartoonyChargeMid.CartoonyChargeMid'"));

	if (OleadaSoundAsset.Succeeded())
		OleadaSound = OleadaSoundAsset.Object;
	// Control global de disparos en oleadas
	IntervaloControlAtaque = 0.25f;

	//// Timer para revisar ataques de naves continuamente
	//GetWorldTimerManager().SetTimer(
	//	TimerHandle_ControlAtaques,
	//	this,
	//	&AAventuraManager::ControlarAtaquesNaves,
	//	IntervaloControlAtaque,
	//	true
	//);

}

void AAventuraManager::BeginPlay()
{
	Super::BeginPlay();

	ShipFactory = NewObject<UShipFactoryGeneral>(this);

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC) return;

	FVector SpawnLocation(-1200.f, 0.f, 300.f);
	FRotator SpawnRotation = FRotator::ZeroRotator;

	AMobileSpacePawn* NewPawn =
		GetWorld()->SpawnActor<AMobileSpacePawn>(AMobileSpacePawn::StaticClass(), SpawnLocation, SpawnRotation);

	if (PC && NewPawn)
	{
		PC->Possess(NewPawn);
	}

	//FVector PlataformaLoc(-116.168701f, 6.159666f, -940.0f);
	//FRotator PlataformaRot(0.f, 0.f, 0.f);
	//FActorSpawnParameters Params;
	//PlataformaSpawn = GetWorld()->SpawnActor<AMegaPlataforma>(AMegaPlataforma::StaticClass(), PlataformaLoc, PlataformaRot, Params);
	//if (PlataformaSpawn)
	//{
	//	PlataformaSpawn->SetActorScale3D(FVector(8.f, 9.75f, 3.f));
	//}

	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle_ControlAtaques,
		this,
		&AAventuraManager::ControlAtaqueAleatorio,
		0.2f,
		true
	);


	SetupFixedCamera();
	ControladorNiveles();
}


void AAventuraManager::SetupFixedCamera()
{
	UWorld* World = GetWorld();
	if (!World) return;

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector CameraLocation(0.f, 0.f, 2000.f);
	FRotator CameraRotation(-90.f, 0.f, 0.f);

	FixedCamera = World->SpawnActor<ACameraActor>(CameraLocation, CameraRotation, Params);
	if (!FixedCamera) return;

	UCameraComponent* Cam = FixedCamera->GetCameraComponent();
	if (!Cam) return;

	// Modo ORTOGRÁFICO
	Cam->SetProjectionMode(ECameraProjectionMode::Orthographic);

	// Ajuste según dispositivo
	const FVector2D ViewportSize = GEngine->GameViewport->Viewport->GetSizeXY();
	float AspectRatio = ViewportSize.X / ViewportSize.Y;

	float BaseWidth = 4000.f;
	float AdjustedWidth = BaseWidth * AspectRatio;

	Cam->SetOrthoWidth(AdjustedWidth);

	APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
	if (PC)
	{
		PC->SetViewTarget(FixedCamera);
	}
}

void AAventuraManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoverJugador(DeltaTime);
}


void AAventuraManager::SetNivelActual(int32 NuevoNivel)
{
	NivelActual = NuevoNivel;
	ControladorNiveles();
}

void AAventuraManager::SiguienteNivel()
{
	NivelActual++;
	ControladorNiveles();
}

void AAventuraManager::ControladorNiveles()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC)
	{
		PC = GetWorld()->SpawnActor<APlayerController>(APlayerController::StaticClass());
		if (!PC) return;
	}

	AHUDmain* HUD = Cast<AHUDmain>(PC->GetHUD());
	if (!HUD)
	{
		HUD = GetWorld()->SpawnActor<AHUDmain>(AHUDmain::StaticClass());
		if (HUD)
			PC->MyHUD = HUD;
		else
			return;
	}

	FString NombreNivel;

	switch (NivelActual)
	{
	case 1: NombreNivel = TEXT("NIVEL 1: Solar Rift"); TeletransportarJugador(); break;
	case 2: NombreNivel = TEXT("NIVEL 2: Ion Core"); TeletransportarJugador(); break;
	case 3: NombreNivel = TEXT("NIVEL 3: Dark Nova"); TeletransportarJugador(); break;
	case 4: NombreNivel = TEXT("NIVEL 4: Quantum Halo"); TeletransportarJugador(); break;
	case 5: NombreNivel = TEXT("NIVEL 5: Infernum Core"); TeletransportarJugador(); break;
	case 6: NombreNivel = TEXT("NIVEL 6: Asteroid Dominion"); TeletransportarJugador(); break;
	case 7: NombreNivel = TEXT("NIVEL 7: Final Radiance"); TeletransportarJugador(); break;
	default: NombreNivel = TEXT("Sector Unknown"); break;
	}

	HUD->MostrarNivel(NombreNivel);

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_IniciarNivel);

	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle_IniciarNivel,
		[this, HUD]()
		{
			switch (NivelActual)
			{
			case 1: Nivel1(); break;
			case 2: Nivel2(); break;
			case 3: Nivel3(); break;
			case 4: Nivel4(); break;
			case 5: Nivel5(); break;
			case 6: Nivel6(); break;
			case 7: Nivel7(); break;
			default: break;
			}
			HUD->OcultarNivel();
		},
		3.0f, false);
}

void AAventuraManager::ActivarEfectoSonidoPantallaCarga(bool bActivarSonido)
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC || !AudioComp_SonidoCarga || !SonidoCarga)
		return;

	if (bActivarSonido)
	{
		if (!AudioComp_SonidoCarga->IsPlaying())
		{
			AudioComp_SonidoCarga->SetSound(SonidoCarga);
			AudioComp_SonidoCarga->Play();
		}

		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle_DetenerSonido,
			[this]()
			{
				ActivarEfectoSonidoPantallaCarga(false);
			},
			4.5f, false);
	}
	else
	{
		if (AudioComp_SonidoCarga->IsPlaying())
		{
			AudioComp_SonidoCarga->Stop();
		}
	}
}


void AAventuraManager::GenerarOleada()
{
	if (OleadaActual >= OleadasTotales)
	{
		SpawnPortalFinal();
		return;
	}

	if (OleadaSound)
		UGameplayStatics::PlaySound2D(GetWorld(), OleadaSound);

	ENaveTipo Tipo = TiposActuales[OleadaActual % TiposActuales.Num()];

	FVector Centro = FVector(1350.f, OleadaActual * 480.f, 300.f);

	TArray<AShip_X*> Nuevas = GenerarEnjambre(Tipo, CantidadPorOleada, Centro, 320.f, 1);

	int32 Total = Nuevas.Num();
	int32 Perseguidores = Total * 0.25f;
	int32 Tiradores = Total * 0.50f;

	for (int32 i = 0; i < Nuevas.Num(); i++)
	{
		AShip_X* S = Nuevas[i];
		if (!S) continue;

		if (i < Perseguidores)
		{
			S->SetRole(EShipRole::Perseguidor);
			S->EnableFire(false);
		}
		else if (i < Perseguidores + Tiradores)
		{
			S->SetRole(EShipRole::Normal);
			S->EnableFire(true);
		}
		else
		{
			S->SetRole(EShipRole::Rafaga);
			S->EnableFire(true);
		}
	}

	OleadaActual++;

	GetWorld()->GetTimerManager().SetTimerForNextTick(
		this,
		&AAventuraManager::ComprobarOleadaGeneral
	);
}


void AAventuraManager::ComprobarOleadaGeneral()
{
	TArray<AActor*> Enemigos;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShip_X::StaticClass(), Enemigos);

	if (Enemigos.Num() == 0)
	{
		if (GEngine)
		GenerarOleada();
	}
	else
	{
		if (GEngine)
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AAventuraManager::ComprobarOleadaGeneral);
	}
}

void AAventuraManager::SpawnPortalFinal()
{
	FVector SpawnLocation(850.f, 0.f, -430.0f);
	FRotator SpawnRotation = FRotator::ZeroRotator;

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AMegaPortal* Portal = GetWorld()->SpawnActor<AMegaPortal>(AMegaPortal::StaticClass(), SpawnLocation, SpawnRotation, Params);
}

void AAventuraManager::GenerarOleadaObstaculos()
{
	if (OleadaActualObstaculos >= OleadasTotalesObstaculos)
	{
		SpawnPortalFinal();
		return;
	}

	for (int32 i = 0; i < CantidadPorOleadaObstaculos; i++)
	{
		FVector SpawnLocation;
		SpawnLocation.X = 1000.f;
		SpawnLocation.Y = FMath::RandRange(-1800.f, 1800.f);
		SpawnLocation.Z = 300.f;

		FRotator SpawnRotation = FRotator::ZeroRotator;

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AMegaObstaculo* NuevoObstaculo = GetWorld()->SpawnActor<AMegaObstaculo>(AMegaObstaculo::StaticClass(), SpawnLocation, SpawnRotation, Params);

		if (NuevoObstaculo)
		{
			float Velocidad = FMath::RandRange(300.f, 900.f);
			NuevoObstaculo->SetVelocidadMovimiento(Velocidad);
		}
	}

	OleadaActualObstaculos++;
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AAventuraManager::ComprobarOleadaObstaculos);
}

void AAventuraManager::ComprobarOleadaObstaculos()
{
	TArray<AActor*> Obstaculos;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMegaObstaculo::StaticClass(), Obstaculos);

	if (Obstaculos.Num() == 0)
	{
		if (OleadaActualObstaculos >= OleadasTotalesObstaculos)
		{
			SpawnPortalFinal();
			return;
		}
		GenerarOleadaObstaculos();
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_ComprobarObstaculos, this, &AAventuraManager::ComprobarOleadaObstaculos, 1.0f, false);
	}
}

void AAventuraManager::TeletransportarJugador()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC)
		return;

	AMobileSpacePawn* PawnJugador = Cast<AMobileSpacePawn>(PC->GetPawn());
	if (!PawnJugador)
		return;

	// Desactivar control del jugador
	PC->SetIgnoreMoveInput(true);
	PC->SetIgnoreLookInput(true);

	// Colocar fuera de la vista
	FVector NuevaPosicion(-2800.f, 0.f, 300.f);
	PawnJugador->SetActorLocation(NuevaPosicion);

	// Iniciar movimiento automático hacia +X
	bJugadorMoviendose = true;
	SetActorTickEnabled(true);
}

void AAventuraManager::MoverJugador(float DeltaTime)
{
	if (!bJugadorMoviendose)
		return;

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC)
		return;

	AMobileSpacePawn* PawnJugador = Cast<AMobileSpacePawn>(PC->GetPawn());
	if (!PawnJugador)
		return;

	FVector Pos = PawnJugador->GetActorLocation();
	Pos.X += VelocidadTeletransporte * DeltaTime;
	PawnJugador->SetActorLocation(Pos);

	if (Pos.X >= -1100.f)
	{
		// Llegó a la posición final
		bJugadorMoviendose = false;
	}
}

void AAventuraManager::SpawnBoss()
{
	FVector BossLocation(1500.f, 0.f, 300.f); 
	FRotator BossRotation = FRotator::ZeroRotator;
	FActorSpawnParameters Params;


	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//ADKraken_Boss_Z* KrakenBoss = GetWorld()->SpawnActor<ADKraken_Boss_Z>(BossLocation, BossRotation, Params);

	//CurrentBoss = KrakenBoss;
}


TArray<AShip_X*> AAventuraManager::GenerarEnjambre(
	ENaveTipo TipoNave,
	int32 Cantidad,
	FVector Centro,
	float Espaciado,
	int32 Filas)
{
	TArray<AShip_X*> SpawnedShips;

	const float SpawnX = MovementMax.X + 500.f; // FUERA DEL BORDE DERECHO
	const float MinDistance = 350.f; // Separación mínima entre naves

	for (int32 i = 0; i < Cantidad; i++)
	{
		float RandY = FMath::FRandRange(MovementMin.Y, MovementMax.Y);
		float RandZ = 300.f;

		FVector SpawnLoc = FVector(SpawnX, RandY, RandZ);

		// Comprobar distancia mínima
		bool bValid = true;
		for (AShip_X* S : SpawnedShips)
		{
			if (S && FVector::Dist(S->GetActorLocation(), SpawnLoc) < MinDistance)
			{
				bValid = false;
				break;
			}
		}

		if (!bValid)
		{
			i--;
			continue;
		}

		AShip_X* Ship = ShipFactory->CrearNave(GetWorld(), TipoNave, SpawnLoc, FRotator(0.f, -180.f, 0.f));
		if (Ship)
		{
			SpawnedShips.Add(Ship);
			RegisterShip(Ship);
		}
	}

	return SpawnedShips;
}

void AAventuraManager::ControlAtaqueAleatorio()
{
	if (ActiveShips.Num() == 0)
		return;

	for (AShip_X* Ship : ActiveShips)
	{
		if (!IsValid(Ship))
			continue;

		// Solo ciertas naves pueden disparar aleatoriamente
		if (!Ship->bPuedeAtacar)
			continue;

		// Probabilidad entre 0 y 1
		float R = FMath::FRand();

		// 8% de probabilidad por tick de disparar
		if (R < 0.08f)
		{
			Ship->FireIfReady();
		}
	}
}


void AAventuraManager::RegisterShip(AShip_X* Ship)
{
	if (!Ship) return;

	ActiveShips.Add(Ship);

	ShipsByType.FindOrAdd(Ship->Tipo).Add(Ship);
	ShipsByRole.FindOrAdd(Ship->ShipRole).Add(Ship);
}


void AAventuraManager::UnregisterShip(AShip_X* Ship)
{
	if (!Ship) return;

	ActiveShips.Remove(Ship);

	if (ShipsByType.Contains(Ship->Tipo))
		ShipsByType[Ship->Tipo].Remove(Ship);

	if (ShipsByRole.Contains(Ship->ShipRole))
		ShipsByRole[Ship->ShipRole].Remove(Ship);
}

TArray<AShip_X*> AAventuraManager::GetShipsOfType(ENaveTipo Tipo)
{
	return ShipsByType.Contains(Tipo) ? ShipsByType[Tipo] : TArray<AShip_X*>();
}

TArray<AShip_X*> AAventuraManager::GetShipsWithRole(EShipRole Rol)
{
	return ShipsByRole.Contains(Rol) ? ShipsByRole[Rol] : TArray<AShip_X*>();
}

void AAventuraManager::EnableAttackForType(ENaveTipo Tipo, bool bEnable)
{
	if (!ShipsByType.Contains(Tipo)) return;

	for (AShip_X* Ship : ShipsByType[Tipo])
		Ship->EnableFire(bEnable);
}

void AAventuraManager::EnableAttackForRole(EShipRole Rol, bool bEnable)
{
	if (!ShipsByRole.Contains(Rol)) return;

	for (AShip_X* Ship : ShipsByRole[Rol])
		Ship->EnableFire(bEnable);
}

bool AAventuraManager::IsWaveCleared() const
{
	return ActiveShips.Num() == 0;
}


void AAventuraManager::Nivel1()
{
	//PlataformaSpawn->CambiarMaterialAleatorio();

	OleadasTotales = 3;                           
	CantidadPorOleada = 8;                         
	TiposActuales = { ENaveTipo::Roja, ENaveTipo::Azul, ENaveTipo::Verde }; 
	OleadaActual = 0;

	GenerarOleada();                               

	//GetWorld()->GetTimerManager().SetTimer(
	//	TimerHandle_SpawnBoss,
	//	this,
	//	&AAventuraManager::SpawnBoss,
	//	10.0f, // Tiempo después de la última oleada
	//	false
	//);
	
}

void AAventuraManager::Nivel2()
{
	//PlataformaSpawn->CambiarMaterialAleatorio();

	OleadaActualObstaculos = 2;
	OleadasTotalesObstaculos = 7;
	CantidadPorOleadaObstaculos = 3;
	GenerarOleadaObstaculos();

	
}


void AAventuraManager::Nivel3()
{
	OleadasTotales = 4;
	CantidadPorOleada = 5;
	TiposActuales = { ENaveTipo::Omega, ENaveTipo::Delta, ENaveTipo::Alfa };
	OleadaActual = 0;
	GenerarOleada();

	
}

void AAventuraManager::Nivel4()
{
	OleadaActualObstaculos = 0;
	OleadasTotalesObstaculos = 12;
	CantidadPorOleadaObstaculos = 3;
	GenerarOleadaObstaculos();
}

void AAventuraManager::Nivel5()
{
	OleadasTotales = 12;
	CantidadPorOleada = 8;
	TiposActuales = { ENaveTipo::Roja, ENaveTipo::Azul, ENaveTipo::Verde };
	OleadaActual = 0;
	GenerarOleada();
}

void AAventuraManager::Nivel6()
{
	OleadasTotales = 12;
	CantidadPorOleada = 8;
	TiposActuales = { ENaveTipo::Roja, ENaveTipo::Azul, ENaveTipo::Verde };
	OleadaActual = 0;
	GenerarOleada();
}

void AAventuraManager::Nivel7()
{
	OleadasTotales = 12;
	CantidadPorOleada = 8;
	TiposActuales = { ENaveTipo::Roja, ENaveTipo::Azul, ENaveTipo::Verde };
	OleadaActual = 0;
	GenerarOleada();
}


