// Fill out your copyright notice in the Description page of Project Settings.

#include "Ship_CazadorOmega.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "AtackComponent.h"
#include "Engine/Engine.h"
#include "TimerManager.h"

AShip_CazadorOmega::AShip_CazadorOmega()
{
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMeshAsset(TEXT("StaticMesh'/Game/StarSparrow/Meshes/Examples/SM_StarSparrow15.SM_StarSparrow15'"));
	if (ShipMeshAsset.Succeeded())
	{
		ShipMesh->SetStaticMesh(ShipMeshAsset.Object);
		ShipMesh->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f)); // Más grande - es el jefe de naves
	}

	// Crear componente de ataque (MoveComp ya se hereda del padre)
	AttackComp = CreateDefaultSubobject<UAtackComponent>(TEXT("AttackComponent"));
}

void AShip_CazadorOmega::BeginPlay()
{
	Super::BeginPlay();
	
	// Configurar rotación inicial
	SetActorRotation(FRotator(0.0f, -180.0f, 0.0f));
	
	// Configurar movimiento - Omega usa patrones complejos, cambia cada 3 segundos
	MoveComp->StartPosition = GetActorLocation();
	MoveComp->StartPattern(EMovementPattern::Elliptical);
	
	// Cambiar patrones dinámicamente
	FTimerHandle PatternTimer;
	GetWorld()->GetTimerManager().SetTimer(PatternTimer, [this]() {
		MoveComp->StartPattern(EMovementPattern::SineWave);
		
		// Y después de otros 3 segundos cambiar a zigzag
		FTimerHandle PatternTimer2;
		GetWorld()->GetTimerManager().SetTimer(PatternTimer2, [this]() {
			MoveComp->StartPattern(EMovementPattern::ZigZag);
		}, 3.0f, false);
	}, 3.0f, false);
	
	// Configurar disparo automático MUY agresivo
	GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AShip_CazadorOmega::AutoFire, TimeBetweenShots, true, 0.5f);
}

void AShip_CazadorOmega::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AShip_CazadorOmega::AutoFire()
{
	if (AttackComp)
	{
		AttackComp->Fire(AttackPattern);
		//if (GEngine)
		//{
		//	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Purple, TEXT("CAZADOR OMEGA FIRED SPREAD! [DANGER]"));
		//}
	}
}
