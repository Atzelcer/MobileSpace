// Fill out your copyright notice in the Description page of Project Settings.


#include "AventuraManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstance.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "EngineUtils.h"


// Sets default values
AAventuraManager::AAventuraManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	NivelActual = 1;
	NivelEnCurso = 1;
	MaterialGameMejor = nullptr;
}

// Called when the game starts or when spawned
void AAventuraManager::BeginPlay()
{
	Super::BeginPlay();
	ControladorNiveles();
}

// Called every frame
void AAventuraManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAventuraManager::EstablecerMaterial(UMaterialInstance* NuevoMaterial)
{
	if (!NuevoMaterial)
	{
		UE_LOG(LogTemp, Warning, TEXT("Material nulo. No se aplicara ningun cambio."));
		return;
	}

	MaterialGameMejor = NuevoMaterial;

	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("No hay referencia valida al mundo."));
		return;
	}

	for (TActorIterator<AActor> It(World); It; ++It)
	{
		TArray<UStaticMeshComponent*> Componentes;
		It->GetComponents<UStaticMeshComponent>(Componentes);

		for (UStaticMeshComponent* Comp : Componentes)
		{
			if (Comp)
				Comp->SetMaterial(0, NuevoMaterial);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Material establecido y aplicado globalmente."));
}


void AAventuraManager::EscogerMaterialAleatorio()
{
	int32 RandomIndex = FMath::RandRange(2, 7);
	UMaterialInstance* MaterialSeleccionado = nullptr;

	switch (RandomIndex)
	{
	case 2:
		MaterialSeleccionado = LoadObject<UMaterialInstance>(nullptr, TEXT("MaterialInstanceConstant'/Game/Space_Creator/Star_Creator/Material_Instances/M_INS_02.M_INS_02'"));
		break;
	case 3:
		MaterialSeleccionado = LoadObject<UMaterialInstance>(nullptr, TEXT("MaterialInstanceConstant'/Game/Space_Creator/Star_Creator/Material_Instances/M_INS_03.M_INS_03'"));
		break;
	case 4:
		MaterialSeleccionado = LoadObject<UMaterialInstance>(nullptr, TEXT("MaterialInstanceConstant'/Game/Space_Creator/Star_Creator/Material_Instances/M_INS_04.M_INS_04'"));
		break;
	case 5:
		MaterialSeleccionado = LoadObject<UMaterialInstance>(nullptr, TEXT("MaterialInstanceConstant'/Game/Space_Creator/Star_Creator/Material_Instances/M_INS_05.M_INS_05'"));
		break;
	case 6:
		MaterialSeleccionado = LoadObject<UMaterialInstance>(nullptr, TEXT("MaterialInstanceConstant'/Game/Space_Creator/Star_Creator/Material_Instances/M_INS_06.M_INS_06'"));
		break;
	case 7:
		MaterialSeleccionado = LoadObject<UMaterialInstance>(nullptr, TEXT("MaterialInstanceConstant'/Game/Space_Creator/Star_Creator/Material_Instances/M_INS_07.M_INS_07'"));
		break;
	default:
		break;
	}

	if (MaterialSeleccionado)
	{
		MaterialGameMejor = MaterialSeleccionado;
		UE_LOG(LogTemp, Warning, TEXT("Material aleatorio seleccionado: M_INS_%d"), RandomIndex);
	}
}


void AAventuraManager::ReproducirMusicaNivel(int32 Nivel)
{

}

void AAventuraManager::ControladorNiveles()
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


void AAventuraManager::Nivel1()
{
	//if (Next_Oleada)
	//{
	//	Enemies_Oleadas = 2;
	//	TopeOleada = Enemies_Oleadas;
	//	OleadaActual = 1;
	//	Current_Level = 2;
	//	Next_Oleada = false;

	//	if (GameStartedWidget)
	//	{
	//		GameStartedWidget->SetOleadaTextVisibility(true);
	//	}
	//	if (PlayerHUD && PlayerHUD->GameStartedWidget)
	//	{
	//		GameStartedWidget = Cast<UW_PDD_Game_Started>(PlayerHUD->GameStartedWidget);
	//		if (GameStartedWidget)
	//		{
	//			GameStartedWidget->UpdateOleada(TopeOleada, OleadaActual);
	//		}
	//	}

	//	SpawnObstacles(150);
	//	SpawnCapsules(35);
	//}

	//if (Enemies_Oleadas > 0 && ActiveEnemies.Num() == 0)
	//{
	//	FVector BaseLocation = FVector(0.0f, 0.0f, 0.0f);
	//	FVector Offset = FVector(2000.0f, 2000.0f, 2000.0f);

	//	SpawnEnemies(AZ_Ship_Vortex::StaticClass(), BaseLocation, Offset, 5, CurrentLife, CurrentSpeed, CurrentDamage);

	//	SpawnEnemies(AZ_Ship_Bomber::StaticClass(), BaseLocation, Offset, 3, CurrentLife + 30.f, CurrentSpeed + 5.f, CurrentDamage + 8.f);

	//	SpawnEnemies(AZ_Ship_Cyclone::StaticClass(), BaseLocation, Offset, 2, CurrentLife + 40.f, CurrentSpeed + 900.f, CurrentDamage + 100.f);

	//	Enemies_Oleadas--;

	//	if (PlayerHUD && PlayerHUD->GameStartedWidget)
	//	{
	//		GameStartedWidget = Cast<UW_PDD_Game_Started>(PlayerHUD->GameStartedWidget);
	//		if (GameStartedWidget)
	//		{
	//			if (TopeOleada == OleadaActual)
	//			{
	//				OleadaActual = TopeOleada;
	//				GameStartedWidget->UpdateOleada(TopeOleada, OleadaActual);
	//			}
	//			else
	//			{
	//				GameStartedWidget->UpdateOleada(TopeOleada, OleadaActual);
	//			}
	//		}
	//	}
	//}
	//else if (Enemies_Oleadas == 0 && ActiveEnemies.Num() == 0)
	//{
	//	if (GameStartedWidget)
	//	{
	//		GameStartedWidget->SetOleadaTextVisibility(false);
	//	}
	//	TArray<AInteractable_Objects_P*> ObstaclesToClear = ActiveObstacles;
	//	TArray<AInteractable_Objects_P*> CapsulesToClear = ActiveCapsules;

	//	// Limpiar los obstáculos y cápsulas fuera de la iteración del array
	//	ClearObstacles(ObstaclesToClear);
	//	ClearCapsules(CapsulesToClear);
	//	FVector BaseLocationBoss = FVector(4000.0f, 0.0f, 210.0f);
	//	FRotator DefaultRotation = FRotator(0.0f, 0.0f, 0.0f);

	//	// Spawn del jefe
	//	AX_Master_Ship_Leviathan* BossShip = GetWorld()->SpawnActor<AX_Master_Ship_Leviathan>(AX_Master_Ship_Leviathan::StaticClass(), BaseLocationBoss, DefaultRotation);

	//	if (BossShip)
	//	{
	//		BossShip->Set_Vida(CurrentLife_Boss);
	//		BossShip->Set_Velocidad_Nave(CurrentSpeed_Boss);
	//		BossShip->Set_Danio_Disparo(CurrentDamage_Boss);
	//	}
	//	Next_Oleada = true;
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
}

void AAventuraManager::Nivel5()
{
}

void AAventuraManager::Nivel6()
{
}

void AAventuraManager::Nivel7()
{
}


