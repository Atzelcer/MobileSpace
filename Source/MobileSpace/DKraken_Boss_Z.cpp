// Fill out your copyright notice in the Description page of Project Settings.


#include "DKraken_Boss_Z.h"

ADKraken_Boss_Z::ADKraken_Boss_Z()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> KrakenMesh(TEXT("SkeletalMesh'/Game/Kraken/Meshes/KRAKEN.KRAKEN'"));
	if (KrakenMesh.Succeeded())
	{
		BossMesh->SetSkeletalMesh(KrakenMesh.Object);
		BossMesh->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));

	}
}
