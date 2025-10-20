// Fill out your copyright notice in the Description page of Project Settings.


#include "O_Comodin.h"

AO_Comodin::AO_Comodin()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshComodinAsset(TEXT("StaticMesh'/Game/SciFi_Props/Models/SM_Box_3.SM_Box_3'"));
	if (MeshComodinAsset.Succeeded())
	{
		MeshObstaculo->SetStaticMesh(MeshComodinAsset.Object);
	}
}
