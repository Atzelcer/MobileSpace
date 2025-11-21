#include "AventuraManager.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Ship_CazadorAlfa.h"
#include "Ship_CazadorDelta.h"
#include "Ship_CazadorVerde.h"
#include "Ship_CazadorRojo.h"
#include "Ship_CazadorAzul.h"
#include "Ship_CazadorOmega.h"
#include "MobileSpacePawn.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "MegaPortal.h"
#include "MegaObstaculo.h"
#include "HUDmain.h"
#include "Boss_1.h"


AAventuraManager::AAventuraManager()
{
	PrimaryActorTick.bCanEverTick = true;

	NivelActual = 1;
	CurrentWave = 1;

	AudioComp_SonidoCarga = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp_SonidoCarga"));
	AudioComp_SonidoCarga->SetupAttachment(RootComponent);
	AudioComp_SonidoCarga->bAutoActivate = false;

	SonidoCarga = LoadObject<USoundWave>(nullptr,
		TEXT("SoundWave'/Game/AuroraSoundTrack/Wav/Interstellar_Drift.Interstellar_Drift'"));

	static ConstructorHelpers::FObjectFinder<USoundBase> OleadaSoundAsset(
		TEXT("SoundWave'/Game/Musica_D/EpicToonSFX/BonusChargeWhooshAudio/CartoonyChargeMid.CartoonyChargeMid'"));

	if (OleadaSoundAsset.Succeeded())
		OleadaSound = OleadaSoundAsset.Object;
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

	// Modo ORTOGR�FICO
	Cam->SetProjectionMode(ECameraProjectionMode::Orthographic);

	// Ajuste seg�n dispositivo
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

	// �AQU�! Sonido de nueva oleada
	if (OleadaSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), OleadaSound);
	}

	ENaveTipo Tipo = TiposActuales[OleadaActual % TiposActuales.Num()];
	FVector centro = FVector(1350.f, OleadaActual * 480.f, 300.f);
	GenerarEnjambre(Tipo, CantidadPorOleada, centro, 320.f, 1);

	OleadaActual++;
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AAventuraManager::ComprobarOleadaGeneral);
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
	FVector SpawnLocation(850.f, 0.f, 300.f);
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

	// Iniciar movimiento autom�tico hacia +X
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
		// Lleg� a la posici�n final
		bJugadorMoviendose = false;
	}
}


void AAventuraManager::GenerarEnjambre(ENaveTipo TipoNave, int32 Cantidad, FVector Centro, float Espaciado, int32 Filas)
{
	int32 Columnas = FMath::CeilToInt((float)Cantidad / (float)Filas);
	int32 NavesCreadas = 0;

	for (int32 fila = 0; fila < Filas && NavesCreadas < Cantidad; fila++)
	{
		for (int32 col = 0; col < Columnas && NavesCreadas < Cantidad; col++, NavesCreadas++)
		{
			FVector Offset(
				0.f,
				(col - Columnas / 2) * Espaciado + ((fila % 2) * Espaciado * 0.5f),
				(fila * Espaciado)
			);
			FVector Loc = Centro + Offset;
			ShipFactory->CrearNave(GetWorld(), TipoNave, Loc, FRotator::ZeroRotator);
		}
	}
}

void AAventuraManager::Nivel1()
{
	// ===== PRUEBA DE BOSS_1 =====
	// En lugar de oleadas normales, spawnear Boss_1 para pruebas
	
	FVector SpawnLocation(1400.0f, 0.0f, 600.0f); // Arriba del jugador
	FRotator SpawnRotation = FRotator::ZeroRotator;
	
	if (GetWorld())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		
		ABoss_1* Boss = GetWorld()->SpawnActor<ABoss_1>(ABoss_1::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
		
		if (Boss)
		{
			// Boss spawneado exitosamente para pruebas
		}
	}
	
	// Comentar el código original para las pruebas:
	/*
	OleadasTotales = 3;
	CantidadPorOleada = 8;
	TiposActuales = { ENaveTipo::Roja, ENaveTipo::Azul, ENaveTipo::Verde };
	OleadaActual = 0;
	GenerarOleada();
	*/
}

void AAventuraManager::Nivel2()
{
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


