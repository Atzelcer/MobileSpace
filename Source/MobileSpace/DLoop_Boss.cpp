// Fill out your copyright notice in the Description page of Project Settings.


#include "DLoop_Boss.h"

ADLoop_Boss::ADLoop_Boss()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BossMeshAsset(TEXT("StaticMesh'/Game/saki/source/SM_SkyFly_v08.SM_SkyFly_v08'"));
	if (BossMeshAsset.Succeeded())
	{
		BossMesh->SetStaticMesh(BossMeshAsset.Object);
		BossMesh->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));

	}
}

void ADLoop_Boss::DispararAtaque()
{
}
