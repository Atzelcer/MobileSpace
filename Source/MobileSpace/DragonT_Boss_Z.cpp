// Fill out your copyright notice in the Description page of Project Settings.


#include "DragonT_Boss_Z.h"

ADragonT_Boss_Z::ADragonT_Boss_Z()
{
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> DragonTMesh(TEXT("SkeletalMesh'/Game/FourEvilDragonsHP/Meshes/DragonTheTerrorBringer/DragonTheTerrorBringerSK.DragonTheTerrorBringerSK'"));
	if (DragonTMesh.Succeeded())
	{
		BossMesh->SetSkeletalMesh(DragonTMesh.Object);
		BossMesh->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));

	}
}
