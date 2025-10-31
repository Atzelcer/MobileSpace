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
#include "MoveComponent.h"

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

	ControladorNiveles();
}

void AAventuraManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckWaveComplete();
}



void AAventuraManager::ControladorNiveles()
{
	
	// Resetear estado del nivel
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
				default: Nivel1(); break; // Fallback
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


void AAventuraManager::Nivel1()
{
	UWorld* World = GetWorld();
	
	ActiveShips.Empty();
	
	if (CurrentWave == 1)
	{
		// Wave 1: 3 CazadorAlfa - ¡MOVIMIENTOS CHISTOSOS!
		FVector SpawnLocation1 = FVector(3200.0f, -300.0f, 254.0f);   // Superior
		FVector SpawnLocation2 = FVector(3800.0f, 0.0f, 254.0f);      // Centro
		FVector SpawnLocation3 = FVector(4400.0f, 300.0f, 254.0f);    // Inferior
		FRotator SpawnRotation = FRotator(0.0f, 180.0f, 0.0f);
		
		AShip_X* Ship1 = World->SpawnActor<AShip_CazadorAlfa>(AShip_CazadorAlfa::StaticClass(), SpawnLocation1, SpawnRotation);
		AShip_X* Ship2 = World->SpawnActor<AShip_CazadorAlfa>(AShip_CazadorAlfa::StaticClass(), SpawnLocation2, SpawnRotation);
		AShip_X* Ship3 = World->SpawnActor<AShip_CazadorAlfa>(AShip_CazadorAlfa::StaticClass(), SpawnLocation3, SpawnRotation);
		
		//// ¡ASIGNAR MOVIMIENTOS CHISTOSOS!
		//if (Ship1 && Ship1->MoveComp) Ship1->MoveComp->StartPattern(EMovementPattern::ChickenBounce);
		//if (Ship2 && Ship2->MoveComp) Ship2->MoveComp->StartPattern(EMovementPattern::Wobble);
		//if (Ship3 && Ship3->MoveComp) Ship3->MoveComp->StartPattern(EMovementPattern::BouncyBall);
		//
		//if (Ship1) ActiveShips.Add(Ship1);
		//if (Ship2) ActiveShips.Add(Ship2);
		//if (Ship3) ActiveShips.Add(Ship3);
		
	}
	else if (CurrentWave == 2)
	{
		// Wave 2: 4 naves - ¡FORMACIÓN DIAMANTE CON MOVIMIENTOS SÚPER CHISTOSOS!
		FVector SpawnLocation1 = FVector(3600.0f, -250.0f, 254.0f);   // Esquina sup-izq
		FVector SpawnLocation2 = FVector(4200.0f, -250.0f, 254.0f);   // Esquina sup-der
		FVector SpawnLocation3 = FVector(3600.0f, 250.0f, 254.0f);    // Esquina inf-izq
		FVector SpawnLocation4 = FVector(4200.0f, 250.0f, 254.0f);    // Esquina inf-der
		FRotator SpawnRotation = FRotator(0.0f, 180.0f, 0.0f);
		
		AShip_X* Ship1 = World->SpawnActor<AShip_CazadorAlfa>(AShip_CazadorAlfa::StaticClass(), SpawnLocation1, SpawnRotation);
		AShip_X* Ship2 = World->SpawnActor<AShip_CazadorDelta>(AShip_CazadorDelta::StaticClass(), SpawnLocation2, SpawnRotation);
		AShip_X* Ship3 = World->SpawnActor<AShip_CazadorAlfa>(AShip_CazadorAlfa::StaticClass(), SpawnLocation3, SpawnRotation);
		AShip_X* Ship4 = World->SpawnActor<AShip_CazadorDelta>(AShip_CazadorDelta::StaticClass(), SpawnLocation4, SpawnRotation);
		
		//// ¡ASIGNAR PATRONES SÚPER CHISTOSOS!
		//if (Ship1 && Ship1->MoveComp) Ship1->MoveComp->StartPattern(EMovementPattern::DrunkWalk);
		//if (Ship2 && Ship2->MoveComp) Ship2->MoveComp->StartPattern(EMovementPattern::Butterfly);  
		//if (Ship3 && Ship3->MoveComp) Ship3->MoveComp->StartPattern(EMovementPattern::Pendulum);
		//if (Ship4 && Ship4->MoveComp) Ship4->MoveComp->StartPattern(EMovementPattern::CrazyEight);
		//
		//if (Ship1) ActiveShips.Add(Ship1);
		//if (Ship2) ActiveShips.Add(Ship2);
		//if (Ship3) ActiveShips.Add(Ship3);
		//if (Ship4) ActiveShips.Add(Ship4);
		
	}
	
}

void AAventuraManager::Nivel2()
{
	UWorld* World = GetWorld();
	if (!World) return;

	
	ActiveShips.Empty();
	
	if (CurrentWave == 1)
	{
		// Wave 1: ¡5 naves con FORMACIÓN CORAZÓN súper chistosa!
		FVector SpawnLocation1 = FVector(3800.0f, -400.0f, 254.0f);  // Punta izquierda
		FVector SpawnLocation2 = FVector(4400.0f, -200.0f, 254.0f);  // Curva izquierda
		FVector SpawnLocation3 = FVector(5000.0f, 0.0f, 254.0f);     // Centro del corazón
		FVector SpawnLocation4 = FVector(5600.0f, 200.0f, 254.0f);   // Curva derecha
		FVector SpawnLocation5 = FVector(6200.0f, 400.0f, 254.0f);   // Punta derecha
		FRotator SpawnRotation = FRotator(0.0f, 180.0f, 0.0f);
		
		AShip_X* Ship1 = World->SpawnActor<AShip_CazadorVerde>(AShip_CazadorVerde::StaticClass(), SpawnLocation1, SpawnRotation);
		AShip_X* Ship2 = World->SpawnActor<AShip_CazadorAzul>(AShip_CazadorAzul::StaticClass(), SpawnLocation2, SpawnRotation);
		AShip_X* Ship3 = World->SpawnActor<AShip_CazadorDelta>(AShip_CazadorDelta::StaticClass(), SpawnLocation3, SpawnRotation);
		AShip_X* Ship4 = World->SpawnActor<AShip_CazadorVerde>(AShip_CazadorVerde::StaticClass(), SpawnLocation4, SpawnRotation);
		AShip_X* Ship5 = World->SpawnActor<AShip_CazadorAzul>(AShip_CazadorAzul::StaticClass(), SpawnLocation5, SpawnRotation);
		
		//// ¡MOVIMIENTOS SÚPER ROMÁNTICOS Y CHISTOSOS!
		//if (Ship1 && Ship1->MoveComp) Ship1->MoveComp->StartPattern(EMovementPattern::HeartShape);
		//if (Ship2 && Ship2->MoveComp) Ship2->MoveComp->StartPattern(EMovementPattern::Butterfly);
		//if (Ship3 && Ship3->MoveComp) Ship3->MoveComp->StartPattern(EMovementPattern::TornadoSpin);
		//if (Ship4 && Ship4->MoveComp) Ship4->MoveComp->StartPattern(EMovementPattern::Butterfly);
		//if (Ship5 && Ship5->MoveComp) Ship5->MoveComp->StartPattern(EMovementPattern::HeartShape);
		//
		//if (Ship1) ActiveShips.Add(Ship1);
		//if (Ship2) ActiveShips.Add(Ship2);
		//if (Ship3) ActiveShips.Add(Ship3);
		//if (Ship4) ActiveShips.Add(Ship4);
		//if (Ship5) ActiveShips.Add(Ship5);
		
	}
	else if (CurrentWave == 2)
	{
		// Wave 2: ¡7 naves FORMACIÓN TORNADO ÉPICA!
		FVector SpawnLocation1 = FVector(4000.0f, -300.0f, 254.0f);  // Tornado exterior
		FVector SpawnLocation2 = FVector(4600.0f, -150.0f, 254.0f);  // Tornado medio
		FVector SpawnLocation3 = FVector(5200.0f, 0.0f, 254.0f);     // OJO DEL TORNADO
		FVector SpawnLocation4 = FVector(4600.0f, 150.0f, 254.0f);   // Tornado medio
		FVector SpawnLocation5 = FVector(4000.0f, 300.0f, 254.0f);   // Tornado exterior
		FVector SpawnLocation6 = FVector(5800.0f, -100.0f, 254.0f);  // Viento lateral
		FVector SpawnLocation7 = FVector(5800.0f, 100.0f, 254.0f);   // Viento lateral
		FRotator SpawnRotation = FRotator(0.0f, 180.0f, 0.0f);
		
		AShip_X* Ship1 = World->SpawnActor<AShip_CazadorRojo>(AShip_CazadorRojo::StaticClass(), SpawnLocation1, SpawnRotation);
		AShip_X* Ship2 = World->SpawnActor<AShip_CazadorDelta>(AShip_CazadorDelta::StaticClass(), SpawnLocation2, SpawnRotation);
		AShip_X* Ship3 = World->SpawnActor<AShip_CazadorRojo>(AShip_CazadorRojo::StaticClass(), SpawnLocation3, SpawnRotation);
		AShip_X* Ship4 = World->SpawnActor<AShip_CazadorDelta>(AShip_CazadorDelta::StaticClass(), SpawnLocation4, SpawnRotation);
		AShip_X* Ship5 = World->SpawnActor<AShip_CazadorRojo>(AShip_CazadorRojo::StaticClass(), SpawnLocation5, SpawnRotation);
		AShip_X* Ship6 = World->SpawnActor<AShip_CazadorVerde>(AShip_CazadorVerde::StaticClass(), SpawnLocation6, SpawnRotation);
		AShip_X* Ship7 = World->SpawnActor<AShip_CazadorAzul>(AShip_CazadorAzul::StaticClass(), SpawnLocation7, SpawnRotation);
		
		//// ¡ASIGNAR MOVIMIENTOS DE TORNADO ÉPICO!
		//if (Ship1 && Ship1->MoveComp) Ship1->MoveComp->StartPattern(EMovementPattern::TornadoSpin);
		//if (Ship2 && Ship2->MoveComp) Ship2->MoveComp->StartPattern(EMovementPattern::TornadoSpin);  
		//if (Ship3 && Ship3->MoveComp) Ship3->MoveComp->StartPattern(EMovementPattern::CrazyEight); // Ojo del tornado
		//if (Ship4 && Ship4->MoveComp) Ship4->MoveComp->StartPattern(EMovementPattern::TornadoSpin);
		//if (Ship5 && Ship5->MoveComp) Ship5->MoveComp->StartPattern(EMovementPattern::TornadoSpin);
		//if (Ship6 && Ship6->MoveComp) Ship6->MoveComp->StartPattern(EMovementPattern::DrunkWalk); // Viento caótico
		//if (Ship7 && Ship7->MoveComp) Ship7->MoveComp->StartPattern(EMovementPattern::DrunkWalk); // Viento caótico
		//
		//if (Ship1) ActiveShips.Add(Ship1);
		//if (Ship2) ActiveShips.Add(Ship2);
		//if (Ship3) ActiveShips.Add(Ship3);
		//if (Ship4) ActiveShips.Add(Ship4);
		//if (Ship5) ActiveShips.Add(Ship5);
		//if (Ship6) ActiveShips.Add(Ship6);
		//if (Ship7) ActiveShips.Add(Ship7);
		
	}
}

void AAventuraManager::Nivel3()
{
	UWorld* World = GetWorld();
	if (!World) return;


	ActiveShips.Empty();
	
	if (CurrentWave == 1)
	{
		// Wave 1: 6 naves - Escuadrón elite con formación diamante
		FVector SpawnLocation1 = FVector(4200.0f, 0.0f, 254.0f);      // Punta del diamante
		FVector SpawnLocation2 = FVector(4800.0f, -250.0f, 254.0f);   // Izquierda
		FVector SpawnLocation3 = FVector(4800.0f, 250.0f, 254.0f);    // Derecha
		FVector SpawnLocation4 = FVector(5400.0f, -125.0f, 254.0f);   // Centro-izq
		FVector SpawnLocation5 = FVector(5400.0f, 125.0f, 254.0f);    // Centro-der
		FVector SpawnLocation6 = FVector(6000.0f, 0.0f, 254.0f);      // Retaguardia
		FRotator SpawnRotation = FRotator(0.0f, 180.0f, 0.0f);
		
		AShip_X* Ship1 = World->SpawnActor<AShip_CazadorRojo>(AShip_CazadorRojo::StaticClass(), SpawnLocation1, SpawnRotation);
		AShip_X* Ship2 = World->SpawnActor<AShip_CazadorVerde>(AShip_CazadorVerde::StaticClass(), SpawnLocation2, SpawnRotation);
		AShip_X* Ship3 = World->SpawnActor<AShip_CazadorAzul>(AShip_CazadorAzul::StaticClass(), SpawnLocation3, SpawnRotation);
		AShip_X* Ship4 = World->SpawnActor<AShip_CazadorDelta>(AShip_CazadorDelta::StaticClass(), SpawnLocation4, SpawnRotation);
		AShip_X* Ship5 = World->SpawnActor<AShip_CazadorRojo>(AShip_CazadorRojo::StaticClass(), SpawnLocation5, SpawnRotation);
		AShip_X* Ship6 = World->SpawnActor<AShip_CazadorVerde>(AShip_CazadorVerde::StaticClass(), SpawnLocation6, SpawnRotation);
		
		if (Ship1) ActiveShips.Add(Ship1);
		if (Ship2) ActiveShips.Add(Ship2);
		if (Ship3) ActiveShips.Add(Ship3);
		if (Ship4) ActiveShips.Add(Ship4);
		if (Ship5) ActiveShips.Add(Ship5);
		if (Ship6) ActiveShips.Add(Ship6);
		
	}
	else if (CurrentWave == 2)
	{
		// Wave 2: ¡INTRODUCE EL CAZADOR OMEGA! 8 naves - FORMACIÓN FINAL ÉPICA
		FVector SpawnLocation1 = FVector(5200.0f, 0.0f, 254.0f);       // OMEGA AL CENTRO - LÍDER
		FVector SpawnLocation2 = FVector(4600.0f, -300.0f, 254.0f);    // Ala izquierda extrema
		FVector SpawnLocation3 = FVector(4600.0f, 300.0f, 254.0f);     // Ala derecha extrema
		FVector SpawnLocation4 = FVector(5800.0f, -200.0f, 254.0f);    // Escolta izq
		FVector SpawnLocation5 = FVector(5800.0f, 200.0f, 254.0f);     // Escolta der
		FVector SpawnLocation6 = FVector(6400.0f, -100.0f, 254.0f);    // Retaguardia izq
		FVector SpawnLocation7 = FVector(6400.0f, 100.0f, 254.0f);     // Retaguardia der
		FVector SpawnLocation8 = FVector(7000.0f, 0.0f, 254.0f);       // Comandante final
		FRotator SpawnRotation = FRotator(0.0f, 180.0f, 0.0f);
		
		// ¡FORMACIÓN FINAL CON 2 OMEGAS!
		AShip_X* Ship1 = World->SpawnActor<AShip_CazadorOmega>(AShip_CazadorOmega::StaticClass(), SpawnLocation1, SpawnRotation);
		AShip_X* Ship2 = World->SpawnActor<AShip_CazadorRojo>(AShip_CazadorRojo::StaticClass(), SpawnLocation2, SpawnRotation);
		AShip_X* Ship3 = World->SpawnActor<AShip_CazadorRojo>(AShip_CazadorRojo::StaticClass(), SpawnLocation3, SpawnRotation);
		AShip_X* Ship4 = World->SpawnActor<AShip_CazadorDelta>(AShip_CazadorDelta::StaticClass(), SpawnLocation4, SpawnRotation);
		AShip_X* Ship5 = World->SpawnActor<AShip_CazadorDelta>(AShip_CazadorDelta::StaticClass(), SpawnLocation5, SpawnRotation);
		AShip_X* Ship6 = World->SpawnActor<AShip_CazadorVerde>(AShip_CazadorVerde::StaticClass(), SpawnLocation6, SpawnRotation);
		AShip_X* Ship7 = World->SpawnActor<AShip_CazadorAzul>(AShip_CazadorAzul::StaticClass(), SpawnLocation7, SpawnRotation);
		AShip_X* Ship8 = World->SpawnActor<AShip_CazadorOmega>(AShip_CazadorOmega::StaticClass(), SpawnLocation8, SpawnRotation);
		
		if (Ship1) ActiveShips.Add(Ship1);
		if (Ship2) ActiveShips.Add(Ship2);
		if (Ship3) ActiveShips.Add(Ship3);
		if (Ship4) ActiveShips.Add(Ship4);
		if (Ship5) ActiveShips.Add(Ship5);
		if (Ship6) ActiveShips.Add(Ship6);
		if (Ship7) ActiveShips.Add(Ship7);
		if (Ship8) ActiveShips.Add(Ship8);
		
	}
}

void AAventuraManager::Nivel4()
{
	// Nivel 4 = versión más difícil del nivel 1
	Nivel1();
}

void AAventuraManager::Nivel5()
{
	// Nivel 5 = versión más difícil del nivel 2
	Nivel2();
}

void AAventuraManager::Nivel6()
{
	// Nivel 6 = versión más difícil del nivel 3
	Nivel3();
}

void AAventuraManager::Nivel7()
{
	// Nivel 7 = mix de todos
	Nivel1();
}


