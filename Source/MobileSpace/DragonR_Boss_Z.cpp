// Fill out your copyright notice in the Description page of Project Settings.


#include "DragonR_Boss_Z.h"

ADragonR_Boss_Z::ADragonR_Boss_Z()
{
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> DragonRMesh(TEXT("SkeletalMesh'/Game/FourEvilDragonsHP/Meshes/DragonTheUsurper/DragonTheUsurperSK.DragonTheUsurperSK'"));
	if (DragonRMesh.Succeeded())
	{
		BossMesh->SetSkeletalMesh(DragonRMesh.Object);
		BossMesh->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	}
}
