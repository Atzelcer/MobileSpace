// Fill out your copyright notice in the Description page of Project Settings.


#include "DBrutal_Boss.h"

ADBrutal_Boss::ADBrutal_Boss()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BossMeshAsset(TEXT("StaticMesh'/Game/red/source/Final.Final'"));
	if (BossMeshAsset.Succeeded())
	{
		BossMesh->SetStaticMesh(BossMeshAsset.Object);
		BossMesh->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));

	}
}

void ADBrutal_Boss::DispararAtaque()
{
}
