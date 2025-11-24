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
#include "MegaPlataforma.h"
#include "Boss.h"
#include "Boss_1.h"
#include "Boss_2.h"
#include "Boss_3.h"
#include "Boss_4.h"
#include "Boss_5.h"
#include "Boss_6.h"

AAventuraManager::AAventuraManager()
{
	PrimaryActorTick.bCanEverTick = true;

	NivelActual = 1;
	CurrentWave = 1;
	
	bOleadasCompletadas = false;
	bJefeActivo = false;
	bJefeEliminado = false;

	// Inicializar sistema de enjambres
	bUseSwarmFormation = true;
	SwarmRows = 2;
	SwarmColumns = 3;
	ShipSpacing = 800.0f;
	RowSpacing = 600.0f;
	EntryDelay = 0.5f;
	NextShipToEnter = 0;
	SwarmEntryTimer = 0.0f;
	bSwarmForming = false;

	AudioComp_SonidoCarga = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp_SonidoCarga"));
	AudioComp_SonidoCarga->SetupAttachment(RootComponent);
	AudioComp_SonidoCarga->bAutoActivate = false;

	SonidoCarga = LoadObject<USoundWave>(nullptr,
		TEXT("SoundWave'/Game/BOSS_SOUNDS/PantallaCarga.PantallaCarga'"));

	IntervaloControlAtaque = 0.25f;


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

	Cam->SetProjectionMode(ECameraProjectionMode::Orthographic);

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
	
	if (bUseSwarmFormation && bSwarmForming)
	{
		UpdateSwarmFormation(DeltaTime);
	}
	
	static float DebugTimer = 0.0f;
	DebugTimer += DeltaTime;
	
	if (DebugTimer >= 2.0f && bJefeActivo)
	{
		DebugTimer = 0.0f;
		
		TArray<AActor*> QuickBossCheck;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABoss::StaticClass(), QuickBossCheck);
		
		if (bJefeActivo && QuickBossCheck.Num() == 0 && !bJefeEliminado)
		{
			bJefeEliminado = true;
			bJefeActivo = false;
			
			
			GetWorldTimerManager().ClearTimer(WaveTimerHandle);
			SpawnPortalFinal();
		}
	}
}
void AAventuraManager::SetNivelActual(int32 NuevoNivel)
{
	NivelActual = NuevoNivel;
	ControladorNiveles();
}

void AAventuraManager::SiguienteNivel()
{
	if (!GetWorld())
	{
		UE_LOG(LogTemp, Error, TEXT("GetWorld() es nulo en SiguienteNivel()"));
		return;
	}
	
	NivelActual++;
	ControladorNiveles();
}

void AAventuraManager::ControladorNiveles()
{
	if (!GetWorld())
	{
		UE_LOG(LogTemp, Error, TEXT("GetWorld() es nulo en ControladorNiveles()"));
		return;
	}

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC)
	{
		UE_LOG(LogTemp, Error, TEXT("No se pudo obtener PlayerController en ControladorNiveles()"));
		return;
	}

	AHUDmain* HUD = Cast<AHUDmain>(PC->GetHUD());
	if (!HUD)
	{
		UE_LOG(LogTemp, Warning, TEXT("HUD no encontrado, intentando crear uno nuevo"));
		HUD = GetWorld()->SpawnActor<AHUDmain>(AHUDmain::StaticClass());
		if (!HUD)
		{
			UE_LOG(LogTemp, Error, TEXT("No se pudo crear HUD en ControladorNiveles()"));
			return;
		}
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
		[this]()
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
			
			APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			if (PC)
			{
				AHUDmain* SafeHUD = Cast<AHUDmain>(PC->GetHUD());
				if (SafeHUD)
				{
					SafeHUD->OcultarNivel();
				}
			}
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
	if (bUseSwarmFormation)
	{
		GenerarEnjambre();
	}
	else
	{
		OleadaActual++;

		for (int32 i = 0; i < CantidadPorOleada; i++)
		{
			int32 RandomIndex = FMath::RandRange(0, TiposActuales.Num() - 1);
			ENaveTipo TipoSeleccionado = TiposActuales[RandomIndex];
			
			float RandomY = FMath::FRandRange(-2500.f, 2500.f);
			float RandomX = FMath::FRandRange(1200.f, 1800.f);
			FVector SpawnLocation(RandomX, RandomY, 300.f);
			FRotator SpawnRotation = FRotator::ZeroRotator;
			
			AShip_X* NewShip = ShipFactory->CrearNave(GetWorld(), TipoSeleccionado, SpawnLocation, SpawnRotation);
		}
	}

	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AAventuraManager::ComprobarOleadaGeneral);
}

void AAventuraManager::GenerarEnjambre()
{
	OleadaActual++;
	
	CurrentSwarmShips.Empty();
	NextShipToEnter = 0;
	SwarmEntryTimer = 0.0f;
	bSwarmForming = true;

	switch (NivelActual)
	{
	case 1:
		SwarmRows = 3; SwarmColumns = 4;  
		break;
	case 2:
		SwarmRows = 4; SwarmColumns = 3;  
		break;
	case 3:
		SwarmRows = 4; SwarmColumns = 4; 
		break;
	case 4:
		SwarmRows = 4; SwarmColumns = 5; 
		break;
	case 5:
		SwarmRows = 5; SwarmColumns = 4; 
		break;
	case 6:
		SwarmRows = 5; SwarmColumns = 5;
		break;
	case 7:
		SwarmRows = 6; SwarmColumns = 4;
		break;
	default:
		SwarmRows = 4; SwarmColumns = 4; 
		break;
	}


	FVector BasePosition(1400.f, 0.f, 300.f);
	
	for (int32 Row = 0; Row < SwarmRows; Row++)
	{
		for (int32 Column = 0; Column < SwarmColumns; Column++)
		{
			if (TiposActuales.Num() == 0) break;
			
			int32 RandomIndex = FMath::RandRange(0, TiposActuales.Num() - 1);
			ENaveTipo TipoSeleccionado = TiposActuales[RandomIndex];
			
			FVector FormationOffset;
			FormationOffset.X = Row * -RowSpacing; 
			FormationOffset.Y = (Column - (SwarmColumns * 0.5f)) * ShipSpacing;
			FormationOffset.Z = 0.f;
			
			
			FVector EntryPosition = BasePosition + FVector(600.f, FormationOffset.Y, 0.f);
			FVector FinalPosition = BasePosition + FormationOffset;
			
		
			
			AShip_X* NewShip = ShipFactory->CrearNave(GetWorld(), TipoSeleccionado, 
													EntryPosition, 
													FRotator(0.f, -180.f, 0.f));
			
			if (NewShip)
			{
				NewShip->bUseFormation = false;
				NewShip->FormationAnchor = FVector::ZeroVector;
				NewShip->FormationOffset = FVector::ZeroVector;
				
				bool bIsFrontLine = (Row == 0);
				NewShip->EnableFire(false); 
				NewShip->SetAttackState(false);
				
				NewShip->SetMovement(EArcadeMovement::SwarmEntry);
				
				if (NewShip->MoveComp)
				{
					FVector TargetFormationPosition = BasePosition + FormationOffset;
					NewShip->MoveComp->SetSwarmTargetPosition(TargetFormationPosition);
					NewShip->MoveComp->SetSwarmPhase(0); 
				}
				
				FVector TargetFormationPosition = BasePosition + FormationOffset;
				
				CurrentSwarmShips.Add(NewShip);
				
			
			}
		}
	}
	
}

void AAventuraManager::UpdateSwarmFormation(float DeltaTime)
{
	SwarmEntryTimer += DeltaTime;
	
	bool bAllInFormation = true;
	int32 NavesEnFormacion = 0;
	
	for (AShip_X* Ship : CurrentSwarmShips)
	{
		if (!Ship || !IsValid(Ship)) continue;
		
		if (Ship->MoveComp && Ship->MoveComp->SwarmPhase < 2)
		{
			bAllInFormation = false;
		}
		else
		{
			NavesEnFormacion++;
		}
	}
	
	if (FMath::Fmod(SwarmEntryTimer, 2.0f) < DeltaTime)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enjambre: %d/%d naves en formación"), NavesEnFormacion, CurrentSwarmShips.Num());
	}
	
	if (bAllInFormation && NavesEnFormacion > 0)
	{
		FTimerHandle DelayTimer;
		GetWorld()->GetTimerManager().SetTimer(DelayTimer, [this]()
		{
			EnableFrontLineAttack();
			bSwarmForming = false;
			UE_LOG(LogTemp, Warning, TEXT("¡ENJAMBRE FORMADO! Habilitando ataque de primera línea"));
		}, 1.5f, false);
	}
}

void AAventuraManager::EnableFrontLineAttack()
{
	for (int32 i = 0; i < CurrentSwarmShips.Num(); i++)
	{
		AShip_X* Ship = CurrentSwarmShips[i];
		if (!Ship || !IsValid(Ship)) continue;
		
		bool bIsFrontLine = (i < SwarmColumns);
		
		if (bIsFrontLine)
		{
			Ship->EnableFire(true);
			Ship->SetAttackState(true);
			Ship->StartAutoFire();
		}
		else
		{
			Ship->EnableFire(false);
			Ship->SetAttackState(false);
		}
	}
}


void AAventuraManager::ComprobarOleadaGeneral()
{
	TArray<AActor*> Enemigos;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShip_X::StaticClass(), Enemigos);

	if (bUseSwarmFormation && CurrentSwarmShips.Num() > 0)
	{
		for (int32 i = CurrentSwarmShips.Num() - 1; i >= 0; i--)
		{
			if (!CurrentSwarmShips[i] || !IsValid(CurrentSwarmShips[i]))
			{
				CurrentSwarmShips.RemoveAt(i);
			}
		}

		bool bHasFrontLineShips = false;
		for (int32 i = 0; i < FMath::Min(SwarmColumns, CurrentSwarmShips.Num()); i++)
		{
			if (CurrentSwarmShips[i] && IsValid(CurrentSwarmShips[i]))
			{
				bHasFrontLineShips = true;
				break;
			}
		}

		if (!bHasFrontLineShips && CurrentSwarmShips.Num() > 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Primera línea destruida! Promoviendo nuevas naves al frente"));
			
			TArray<AShip_X*> SurvivingShips;
			for (AShip_X* Ship : CurrentSwarmShips)
			{
				if (Ship && IsValid(Ship))
				{
					SurvivingShips.Add(Ship);
				}
			}
			CurrentSwarmShips = SurvivingShips;
			
			EnableFrontLineAttack();
		}
	}

	if (Enemigos.Num() == 0 && !bOleadasCompletadas)
	{
		CurrentSwarmShips.Empty();
		bSwarmForming = false;
		
		if (OleadaActual < OleadasTotales)
		{
			GenerarOleada();
		}
		else
		{
			bOleadasCompletadas = true;

			GetWorldTimerManager().SetTimer(BossTimerHandle, this, &AAventuraManager::SpawnJefeDelNivel, 5.0f, false);

			ComprobarJefe();
		}
	}
	else if (bOleadasCompletadas && !bJefeEliminado)
	{

		
		if (bJefeActivo && !bJefeEliminado)
		{
			TArray<AActor*> ManualBossCheck;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABoss::StaticClass(), ManualBossCheck);
			
			if (ManualBossCheck.Num() == 0)
			{
				bJefeEliminado = true;
				bJefeActivo = false;

				if (NivelActual == 7)
				{
					APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
					if (PC)
					{
						AHUDmain* HUD = Cast<AHUDmain>(PC->GetHUD());
						if (HUD)
						{
							HUD->MostrarAjustes();
						}
					}

					return;
				}
								
				GetWorldTimerManager().ClearTimer(WaveTimerHandle);
				GetWorldTimerManager().SetTimer(WaveTimerHandle, this, &AAventuraManager::SpawnPortalFinal, 1.0f, false);
			}
		}
		
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AAventuraManager::ComprobarOleadaGeneral);
	}
	else if (Enemigos.Num() > 0)
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AAventuraManager::ComprobarOleadaGeneral);
	}
}

void AAventuraManager::SpawnPortalFinal()
{
	
	TArray<AActor*> ExistingPortals;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMegaPortal::StaticClass(), ExistingPortals);
	
	if (ExistingPortals.Num() > 0)
	{
		
		for (AActor* Portal : ExistingPortals)
		{
			if (Portal)
			{
				Portal->Destroy();
			}
		}
	}
	
	FVector SpawnLocation(850.f, 0.f, -430.0f);
	FRotator SpawnRotation = FRotator::ZeroRotator;


	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AMegaPortal* Portal = GetWorld()->SpawnActor<AMegaPortal>(AMegaPortal::StaticClass(), SpawnLocation, SpawnRotation, Params);
	
	if (Portal)
	{
		UE_LOG(LogTemp, Warning, TEXT("AventuraManager: Portal spawneado exitosamente"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AventuraManager: Error al spawnear portal, intentando método alternativo"));
		
		FActorSpawnParameters AlternativeParams;
		AlternativeParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AlternativeParams.bNoFail = true;
		
		FVector AlternativeLocation(800.f, 0.f, -400.0f);
		
		AMegaPortal* AlternativePortal = GetWorld()->SpawnActor<AMegaPortal>(
			AMegaPortal::StaticClass(), 
			AlternativeLocation, 
			SpawnRotation, 
			AlternativeParams);
		
		
	}
	
	TArray<AActor*> PortalsAfterSpawn;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMegaPortal::StaticClass(), PortalsAfterSpawn);
	
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

	PC->SetIgnoreMoveInput(true);
	PC->SetIgnoreLookInput(true);

	FVector NuevaPosicion(-2800.f, 0.f, 300.f);
	PawnJugador->SetActorLocation(NuevaPosicion);

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
		bJugadorMoviendose = false;
	}
}

void AAventuraManager::Nivel1()
{

	OleadasTotales = 3;                           
	CantidadPorOleada = 8;                         
	TiposActuales = { ENaveTipo::Roja, ENaveTipo::Azul, ENaveTipo::Verde }; 
	OleadaActual = 0;
	
	bOleadasCompletadas = false;
	bJefeActivo = false;
	bJefeEliminado = false;

	GenerarOleada();                               

	
}

void AAventuraManager::Nivel2()
{
	OleadasTotales = 4;
	CantidadPorOleada = 5;
	TiposActuales = { ENaveTipo::Omega, ENaveTipo::Delta, ENaveTipo::Alfa };
	OleadaActual = 0;

	bOleadasCompletadas = false;
	bJefeActivo = false;
	bJefeEliminado = false;

	GenerarOleada();
}


void AAventuraManager::Nivel3()
{
	OleadasTotales = 4;
	CantidadPorOleada = 5;
	TiposActuales = { ENaveTipo::Omega, ENaveTipo::Delta, ENaveTipo::Alfa };
	OleadaActual = 0;
	
	bOleadasCompletadas = false;
	bJefeActivo = false;
	bJefeEliminado = false;
	
	GenerarOleada();

	
}

void AAventuraManager::Nivel4()
{
	OleadasTotales = 6;
	CantidadPorOleada = 6;
	TiposActuales = { ENaveTipo::Roja, ENaveTipo::Azul, ENaveTipo::Verde, ENaveTipo::Omega };
	OleadaActual = 0;
	
	bOleadasCompletadas = false;
	bJefeActivo = false;
	bJefeEliminado = false;
	
	GenerarOleada();
}

void AAventuraManager::Nivel5()
{
	OleadasTotales = 8;
	CantidadPorOleada = 7;
	TiposActuales = { ENaveTipo::Roja, ENaveTipo::Azul, ENaveTipo::Verde, ENaveTipo::Omega, ENaveTipo::Delta };
	OleadaActual = 0;
	
	bOleadasCompletadas = false;
	bJefeActivo = false;
	bJefeEliminado = false;
	
	GenerarOleada();
}

void AAventuraManager::Nivel6()
{
	OleadasTotales = 10;
	CantidadPorOleada = 8;
	TiposActuales = { ENaveTipo::Roja, ENaveTipo::Azul, ENaveTipo::Verde };
	OleadaActual = 0;
	bOleadasCompletadas = false;
	bJefeActivo = false;
	bJefeEliminado = false;
	GenerarOleada();
}

void AAventuraManager::Nivel7()
{
	OleadasTotales = 12;
	CantidadPorOleada = 8;
	TiposActuales = { ENaveTipo::Roja, ENaveTipo::Azul, ENaveTipo::Verde };
	OleadaActual = 0;
	bOleadasCompletadas = false;
	bJefeActivo = false;
	bJefeEliminado = false;
	GenerarOleada();
}


void AAventuraManager::SpawnJefeDelNivel()
{
	int32 TipoJefe = GetTipoJefeParaNivel(NivelActual);
	
	FVector SpawnLocation = FVector(1400.0f, 0.0f, 300.0f);
	FRotator SpawnRotation = FRotator(0.0f, 180.0f, 0.0f);
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	
	ABoss* NuevoJefe = nullptr;
	
	switch (TipoJefe)
	{
	case 1:
		NuevoJefe = GetWorld()->SpawnActor<ABoss_1>(SpawnLocation, SpawnRotation, SpawnParams);
		break;
	case 2:
		NuevoJefe = GetWorld()->SpawnActor<ABoss_2>(SpawnLocation, SpawnRotation, SpawnParams);
		break;
	case 3:
		NuevoJefe = GetWorld()->SpawnActor<ABoss_3>(SpawnLocation, SpawnRotation, SpawnParams);
		break;
	case 4:
		NuevoJefe = GetWorld()->SpawnActor<ABoss_4>(SpawnLocation, SpawnRotation, SpawnParams);
		break;
	case 5:
		NuevoJefe = GetWorld()->SpawnActor<ABoss_5>(SpawnLocation, SpawnRotation, SpawnParams);
		break;
	case 6:
		NuevoJefe = GetWorld()->SpawnActor<ABoss_6>(SpawnLocation, SpawnRotation, SpawnParams);
		break;
	default:
		NuevoJefe = GetWorld()->SpawnActor<ABoss_1>(SpawnLocation, SpawnRotation, SpawnParams);
		break;
	}
	
	if (NuevoJefe)
	{
		bJefeActivo = true;
		bJefeEliminado = false;
		
	}
	
}

void AAventuraManager::ComprobarJefe()
{

	TArray<AActor*> FoundBosses;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABoss::StaticClass(), FoundBosses);
	
	
	if (FoundBosses.Num() == 0)
	{
		bJefeEliminado = true;
		bJefeActivo = false;
				
		GetWorldTimerManager().ClearTimer(WaveTimerHandle);
		GetWorldTimerManager().SetTimer(WaveTimerHandle, this, &AAventuraManager::SpawnPortalFinal, 3.0f, false);
	}
}

int32 AAventuraManager::GetTipoJefeParaNivel(int32 Nivel)
{
	switch (Nivel)
	{
	case 1:
		return 1; 
	case 2:
		return 2; 
	case 3:
		return 1; 
	case 4:
		return 3; 
	case 5:
		return 4; 
	case 6:
		return 5;
	case 7:
		return 6; 
	default:
		return 1; 
	}
}

void AAventuraManager::DebugEstadoJefe()
{
	TArray<AActor*> FoundBosses;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABoss::StaticClass(), FoundBosses);
	
	
	if (bJefeActivo && !bJefeEliminado && FoundBosses.Num() == 0)
	{
		
		bJefeEliminado = true;
		bJefeActivo = false;
		
		GetWorldTimerManager().ClearTimer(WaveTimerHandle);
		GetWorldTimerManager().SetTimer(WaveTimerHandle, this, &AAventuraManager::SpawnPortalFinal, 1.0f, false);
	}
}

void AAventuraManager::ForzarEliminacionJefe()
{
	TArray<AActor*> FoundBosses;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABoss::StaticClass(), FoundBosses);
	
	for (AActor* BossActor : FoundBosses)
	{
		if (BossActor)
		{
			BossActor->Destroy();
		}
	}
	
	bJefeEliminado = true;
	bJefeActivo = false;
	
	GetWorldTimerManager().ClearTimer(WaveTimerHandle);
	SpawnPortalFinal();
}


