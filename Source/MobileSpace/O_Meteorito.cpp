// Fill out your copyright notice in the Description page of Project Settings.


#include "O_Meteorito.h"

AO_Meteorito::AO_Meteorito()
{
	PrimaryActorTick.bCanEverTick = true;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/RockEnv_Pack/Meshes/Cave_Rocks/SM_Rock_Cave_3.SM_Rock_Cave_3'"));
	if (MeshAsset.Succeeded())
	{
		MeshObstaculo->SetStaticMesh(MeshAsset.Object);
	}
}
