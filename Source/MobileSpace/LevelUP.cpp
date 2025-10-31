// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelUP.h"
#include "Particles/ParticleSystemcomponent.h"


ALevelUP::ALevelUP()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>CapsulaMeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_NarrowCapsule.Shape_NarrowCapsule'"));
	if (CapsulaMeshAsset.Succeeded())
	{
		CapsulaMesh->SetStaticMesh(CapsulaMeshAsset.Object);
		CapsulaMesh->SetVisibility(false);

	}
	

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("ParticleSystem'/Game/VE_PowerUps/Particles/Capsule/Evolution/PS_04_Y_CogwheelSpawn.PS_04_Y_CogwheelSpawn'"));
	if (ParticleAsset.Succeeded())
	{
		LevelUpEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LevelUpEffect"));
		LevelUpEffect->SetupAttachment(RootComponent);
		LevelUpEffect->SetTemplate(ParticleAsset.Object);
	}
}
