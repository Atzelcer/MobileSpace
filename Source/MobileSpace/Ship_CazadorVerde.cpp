// Fill out your copyright notice in the Description page of Project Settings.


#include "Ship_CazadorVerde.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "MoveComponent.h"
#include "AtackComponent.h"
#include "Engine/Engine.h"
#include "TimerManager.h"

AShip_CazadorVerde::AShip_CazadorVerde()
{
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMeshAsset(TEXT("StaticMesh'/Game/StarSparrow/Meshes/Examples/SM_StarSparrow05.SM_StarSparrow05'"));
	if (ShipMeshAsset.Succeeded())
	{
		ShipMesh->SetStaticMesh(ShipMeshAsset.Object);
		ShipMesh->SetRelativeScale3D(FVector(0.4f, 0.4f, 0.4f));
	}
	// En el constructor .cpp
	MoveComp = CreateDefaultSubobject<UMoveComponent>(TEXT("MoveComponent"));

	AttackComp = CreateDefaultSubobject<UAtackComponent>(TEXT("AttackComponent"));

	
}

void AShip_CazadorVerde::BeginPlay()
{
	Super::BeginPlay();
	SetActorRotation(FRotator(0.0f, -180.0f, 0.0f));
	// Configurar posición inicial
	SetActorRotation(FRotator(0.0f, -180.0f, 0.0f));

	MoveComp->StartPosition = GetActorLocation();

	// Probar diferentes patrones
	MoveComp->StartPattern(EMovementPattern::SineWave); // Onda sinusoidal

	// O cambiar cada 5 segundos
	FTimerHandle PatternTimer;
	GetWorld()->GetTimerManager().SetTimer(PatternTimer, [this]() {
		MoveComp->StartPattern(EMovementPattern::Loop);
		}, 5.0f, false);


	GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AShip_CazadorVerde::AutoFire, TimeBetweenShots, true, 1.0f);
}

void AShip_CazadorVerde::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AShip_CazadorVerde::AutoFire()
{
	if (AttackComp)
	{
		AttackComp->Fire(AttackPattern);
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Cazador Verde Fired!"));
	}
}
