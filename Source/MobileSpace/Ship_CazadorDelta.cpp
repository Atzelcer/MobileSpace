// Fill out your copyright notice in the Description page of Project Settings.

#include "Ship_CazadorDelta.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "AtackComponent.h"
#include "Engine/Engine.h"
#include "TimerManager.h"

AShip_CazadorDelta::AShip_CazadorDelta()
{
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMeshAsset(TEXT("StaticMesh'/Game/StarSparrow/Meshes/Examples/SM_StarSparrow19.SM_StarSparrow19'"));
	if (ShipMeshAsset.Succeeded())
	{
		ShipMesh->SetStaticMesh(ShipMeshAsset.Object);
		ShipMesh->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));
	}

	AttackComp = CreateDefaultSubobject<UAtackComponent>(TEXT("AttackComponent"));
}

void AShip_CazadorDelta::BeginPlay()
{
	Super::BeginPlay();
	
	SetActorRotation(FRotator(0.0f, -180.0f, 0.0f));
	GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AShip_CazadorDelta::AutoFire, TimeBetweenShots, true, 1.5f);
}

void AShip_CazadorDelta::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AShip_CazadorDelta::AutoFire()
{
	if (AttackComp)
	{
		AttackComp->Fire(AttackPattern);	
	}
}
