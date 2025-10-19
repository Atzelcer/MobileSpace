// Fill out your copyright notice in the Description page of Project Settings.


#include "O_Bombas.h"

AO_Bombas::AO_Bombas()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/SciFi_Props/Models/SM_Box_8.SM_Box_8'"));
	if (MeshAsset.Succeeded())
	{
		MeshObstaculo->SetStaticMesh(MeshAsset.Object);
	}
}
