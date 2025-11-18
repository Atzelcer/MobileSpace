// Fill out your copyright notice in the Description page of Project Settings.


#include "DColossal_Boss.h"

ADColossal_Boss::ADColossal_Boss()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BossMeshAsset(TEXT("StaticMesh'/Game/tru/textures/fast_texturing_output.fast_texturing_output'"));
	if (BossMeshAsset.Succeeded())
	{
		BossMesh->SetStaticMesh(BossMeshAsset.Object);
		BossMesh->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));

	}
}

void ADColossal_Boss::DispararAtaque()
{
}
