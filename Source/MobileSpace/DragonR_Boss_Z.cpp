// Fill out your copyright notice in the Description page of Project Settings.

#include "DragonR_Boss_Z.h"

ADragonR_Boss_Z::ADragonR_Boss_Z()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BossMeshAsset(TEXT("StaticMesh'/Game/saki/source/SM_SkyFly_v07.SM_SkyFly_v07'"));
	if (BossMeshAsset.Succeeded())
	{
		BossMesh->SetStaticMesh(BossMeshAsset.Object);
		BossMesh->SetWorldScale3D(FVector(3.f, 3.f, 3.f));
	}
}

void ADragonR_Boss_Z::DispararAtaque()
{
}


