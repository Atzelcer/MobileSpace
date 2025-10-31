// Fill out your copyright notice in the Description page of Project Settings.


#include "Life.h"
#include "UObject/ConstructorHelpers.h"
#include "Particles/ParticleSystemComponent.h"


ALife::ALife()
{

	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>CapsulaMeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_NarrowCapsule.Shape_NarrowCapsule'"));
	if (CapsulaMeshAsset.Succeeded())
	{
		CapsulaMesh->SetStaticMesh(CapsulaMeshAsset.Object);
		CapsulaMesh->SetVisibility(false);
		
	}


	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	ParticleSystem->SetupAttachment(RootComponent);
	//particle system
	static ConstructorHelpers::FObjectFinder<UParticleSystem>ParticleAsset(TEXT("ParticleSystem'/Game/VE_PowerUps/Particles/Capsule/LIfe/PS_05_R_HeartSpawn.PS_05_R_HeartSpawn'"));
	if (ParticleAsset.Succeeded())
	{
		ParticleSystem->SetTemplate(ParticleAsset.Object);
	}
}
