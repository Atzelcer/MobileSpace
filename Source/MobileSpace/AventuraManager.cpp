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


AAventuraManager::AAventuraManager()
{
	PrimaryActorTick.bCanEverTick = true;
	NivelActual = 1;
	CurrentWave = 1;
	CurrentBoss = nullptr;
}

void AAventuraManager::BeginPlay()
{
	Super::BeginPlay();
	ShipFactory = NewObject<UShipFactoryGeneral>(this);

	
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
	
	ShipFactory->CrearNave(GetWorld(), ENaveTipo::Verde);

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


