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
		ShipMesh->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	}

	AttackComp = CreateDefaultSubobject<UAtackComponent>(TEXT("AttackComponent"));
}

void AShip_CazadorOmega::BeginPlay()
{
	Super::BeginPlay();
	SetActorRotation(FRotator(0.0f, -180.0f, 0.0f));
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
	}
}

void AShip_CazadorOmega::DestruirNave()
{
	Super::DestruirNave();
}
