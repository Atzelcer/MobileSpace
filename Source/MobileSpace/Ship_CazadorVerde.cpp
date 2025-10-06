// Fill out your copyright notice in the Description page of Project Settings.


#include "Ship_CazadorVerde.h"

AShip_CazadorVerde::AShip_CazadorVerde()
{
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMeshAsset(TEXT("StaticMesh'/Game/StarSparrow/Meshes/Examples/SM_StarSparrow05.SM_StarSparrow05'"));
	if (ShipMeshAsset.Succeeded())
	{
		ShipMesh->SetStaticMesh(ShipMeshAsset.Object);
		ShipMesh->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));
		
	}
}
