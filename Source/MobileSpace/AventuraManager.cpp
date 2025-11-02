#include "AventuraManager.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Ship_CazadorAlfa.h"
#include "Ship_CazadorDelta.h"
#include "Ship_CazadorVerde.h"
#include "Ship_CazadorRojo.h"
#include "Ship_CazadorAzul.h"
#include "Ship_CazadorOmega.h"
#include "DKraken_Boss_Z.h"
#include "DragonR_Boss_Z.h"
#include "DragonT_Boss_Z.h"
#include "MobileSpacePawn.h"
#include "Kismet/GameplayStatics.h"
#include "MegaCapsula.h"
#include "MegaObstaculo.h"

AAventuraManager::AAventuraManager()
{
	PrimaryActorTick.bCanEverTick = true;
	NivelActual = 1;
	CurrentWave = 1;
	CurrentBoss = nullptr;


	//DefaultPawnClass = AMobileSpacePawn::StaticClass();
}

void AAventuraManager::BeginPlay()
{
	Super::BeginPlay();

	//UWorld* World = GetWorld();
	//if (World)
	//{
	//	FActorSpawnParameters Params;
	//	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//	Params.Owner = this;

	//	FVector SpawnLocation = FVector::ZeroVector;
	//	FRotator SpawnRotation = FRotator::ZeroRotator;

	//	AMobileSpacePawn* Pawn = World->SpawnActor<AMobileSpacePawn>(AMobileSpacePawn::StaticClass(), SpawnLocation, SpawnRotation, Params);

	//	APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
	//	if (PC && Pawn)
	//		PC->Possess(Pawn);
	//}
	
	ControladorNiveles();
}

void AAventuraManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckWaveComplete();
}



void AAventuraManager::ControladorNiveles()
{

	CurrentWave = 1;
	CurrentBoss = nullptr;
	ActiveShips.Empty();
	
	
	switch (NivelActual)
	{
	case 1: Nivel1(); break;
	case 2: Nivel2(); break;
	case 3: Nivel3(); break;
	default: 
		NivelActual = 1;
		Nivel1(); 
		break;
	}
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

void AAventuraManager::CheckWaveComplete()
{
	UWorld* World = GetWorld();
	if (!World) return;

	ActiveShips.RemoveAll([](AShip_X* Ship) {
		return !IsValid(Ship);
	});

	if (ActiveShips.Num() == 0)
	{
		if (CurrentWave == 1)
		{
			
			CurrentWave = 2;
			GetWorldTimerManager().SetTimer(WaveTimerHandle, [this]() {
				switch (NivelActual)
				{
				case 1: Nivel1(); break;
				case 2: Nivel2(); break;
				case 3: Nivel3(); break;
				default: Nivel1(); break;
				}
			}, 2.0f, false);
		}
		else if (CurrentWave == 2)
		{
			
			CurrentWave = 3;
			
			
			
			GetWorldTimerManager().SetTimer(WaveTimerHandle, this, &AAventuraManager::SiguienteNivel, 2.5f, false);
		}
	}
}

//patrones factory - patron prototype 
void AAventuraManager::Nivel1()
{
	//solo spawnea una nave
	UWorld* World = GetWorld();
	if (!World) return;
	FVector SpawnLocation = FVector(1000.0f, 0.0f, 300.0f);
	FRotator SpawnRotation = FRotator::ZeroRotator;
	
	AShip_CazadorRojo* NewShip = World->SpawnActor<AShip_CazadorRojo>(AShip_CazadorRojo::StaticClass(), SpawnLocation, SpawnRotation);
	
	AShip_CazadorAzul* NewShip1 = World->SpawnActor<AShip_CazadorAzul>(AShip_CazadorAzul::StaticClass(), SpawnLocation, SpawnRotation);
	AShip_CazadorAlfa* NewShip2 = World->SpawnActor<AShip_CazadorAlfa>(AShip_CazadorAlfa::StaticClass(), SpawnLocation, SpawnRotation);


	//const float PosZ = 282.000183f;

	//// Rango Y de aparición
	//const float MinY = -1680.0f;
	//const float MaxY = 1750.0f;

	//// Cuántos spawnear
	//const int32 NumCapsulas = 4;
	//const int32 NumObstaculos = 6;

	//for (int32 i = 0; i < NumCapsulas; ++i)
	//{
	//	float RandY = FMath::FRandRange(MinY, MaxY);
	//	FVector SpawnPos(6000.f + i * 800.f, RandY, PosZ);
	//	World->SpawnActor<AMegaCapsula>(AMegaCapsula::StaticClass(), SpawnPos, FRotator::ZeroRotator);
	//}

	//for (int32 i = 0; i < NumObstaculos; ++i)
	//{
	//	float RandY = FMath::FRandRange(MinY, MaxY);
	//	FVector SpawnPos(6000.f + i * 700.f, RandY, PosZ);
	//	World->SpawnActor<AMegaObstaculo>(AMegaObstaculo::StaticClass(), SpawnPos, FRotator::ZeroRotator);
	//}
}

void AAventuraManager::Nivel2()
{
	
}

void AAventuraManager::Nivel3()
{
	
}

void AAventuraManager::Nivel4()
{
	Nivel1();
}

void AAventuraManager::Nivel5()
{
	Nivel2();
}

void AAventuraManager::Nivel6()
{
	Nivel3();
}

void AAventuraManager::Nivel7()
{
	Nivel1();
}


