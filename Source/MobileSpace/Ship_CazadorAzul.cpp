// Fill out your copyright notice in the Description page of Project Settings.

#include "Ship_CazadorAzul.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "AtackComponent.h"
#include "Engine/Engine.h"
#include "TimerManager.h"

AShip_CazadorAzul::AShip_CazadorAzul()
{
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMeshAsset(TEXT("StaticMesh'/Game/StarSparrow/Meshes/Examples/SM_StarSparrow02.SM_StarSparrow02'"));
	if (ShipMeshAsset.Succeeded())
	{
		ShipMesh->SetStaticMesh(ShipMeshAsset.Object);
		ShipMesh->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f)); // Aumenté el tamaño
	}

	// Crear componente de ataque (MoveComp ya se hereda del padre)
	AttackComp = CreateDefaultSubobject<UAtackComponent>(TEXT("AttackComponent"));
}

void AShip_CazadorAzul::BeginPlay()
{
	Super::BeginPlay();
	
	// Configurar rotación inicial
	SetActorRotation(FRotator(0.0f, -180.0f, 0.0f));
	
	// Configurar movimiento - Azul usa movimiento sine wave
	MoveComp->StartPosition = GetActorLocation();
	MoveComp->StartPattern(EMovementPattern::SineWave);
	
	// Configurar disparo automático rápido
	GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AShip_CazadorAzul::AutoFire, TimeBetweenShots, true, 1.2f);
}

void AShip_CazadorAzul::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AShip_CazadorAzul::AutoFire()
{
	if (AttackComp)
	{
		AttackComp->Fire(AttackPattern);
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, TEXT("Cazador Azul Fired!"));
		}
	}
}
