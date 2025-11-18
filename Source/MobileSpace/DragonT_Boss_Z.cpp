// Fill out your copyright notice in the Description page of Project Settings.

#include "DragonT_Boss_Z.h"

ADragonT_Boss_Z::ADragonT_Boss_Z()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BossMeshAsset(TEXT("StaticMesh'/Game/intrepid/source/Intrepid_Type.Intrepid_Type'"));
	if (BossMeshAsset.Succeeded())
	{
		BossMesh->SetStaticMesh(BossMeshAsset.Object);
		BossMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
		BossMesh->SetWorldScale3D(FVector(5.f, 5.f, 5.f));
	}

}

void ADragonT_Boss_Z::DispararAtaque()
{
}


