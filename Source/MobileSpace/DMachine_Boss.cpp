// Fill out your copyright notice in the Description page of Project Settings.


#include "DMachine_Boss.h"

ADMachine_Boss::ADMachine_Boss()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BossMeshAsset(TEXT("StaticMesh'/Game/intrepid/source/Intrepid_Type1.Intrepid_Type1'"));
	if (BossMeshAsset.Succeeded())
	{
		BossMesh->SetStaticMesh(BossMeshAsset.Object);
		BossMesh->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));

	}
}

void ADMachine_Boss::DispararAtaque()
{
}
