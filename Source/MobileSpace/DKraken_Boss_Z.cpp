// Fill out your copyright notice in the Description page of Project Settings.

#include "DKraken_Boss_Z.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

ADKraken_Boss_Z::ADKraken_Boss_Z()
{
	PrimaryActorTick.bCanEverTick = true;

	//// Setup Kraken-specific mesh
	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> KrakenMesh(TEXT("SkeletalMesh'/Game/Kraken/Meshes/KRAKEN.KRAKEN'"));
	//if (KrakenMesh.Succeeded())
	//{
	//	BossMesh->SetSkeletalMesh(KrakenMesh.Object);
	//}

	//// Load Kraken-specific animations
	//static ConstructorHelpers::FObjectFinder<UAnimSequence> IdleAnim(TEXT("AnimSequence'/Game/Kraken/Animations/KRAKEN_idle.KRAKEN_idle'"));
	//if (IdleAnim.Succeeded())
	//{
	//	IdleAnimation = IdleAnim.Object;
	//	
	//}
	//
	//static ConstructorHelpers::FObjectFinder<UAnimSequence> AttackAnim(TEXT("AnimSequence'/Game/Kraken/Animations/KRAKEN_smashAttack.KRAKEN_smashAttack'"));
	//if (AttackAnim.Succeeded())
	//{
	//	AttackAnimation = AttackAnim.Object;
	//	
	//}
	//

	//static ConstructorHelpers::FObjectFinder<UAnimSequence> DeathAnim(TEXT("AnimSequence'/Game/Kraken/Animations/KRAKEN_death.KRAKEN_death'"));
	//if (DeathAnim.Succeeded())
	//{
	//	DeathAnimation = DeathAnim.Object;
	//	
	//}
}


