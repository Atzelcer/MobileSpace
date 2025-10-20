// Fill out your copyright notice in the Description page of Project Settings.


#include "O_Asteroide.h"

AO_Asteroide::AO_Asteroide()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> AsteroideMesh(TEXT("StaticMesh'/Game/RockEnv_Pack/Meshes/Rocks/SM_Rock_11.SM_Rock_11'"));
	if (AsteroideMesh.Succeeded())
	{
		MeshObstaculo->SetStaticMesh(AsteroideMesh.Object);
	}
}
