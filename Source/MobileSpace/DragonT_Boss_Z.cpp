// Fill out your copyright notice in the Description page of Project Settings.

#include "DragonT_Boss_Z.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

ADragonT_Boss_Z::ADragonT_Boss_Z()
{
	PrimaryActorTick.bCanEverTick = true;

	// Setup DragonT-specific mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> DragonTMesh(TEXT("SkeletalMesh'/Game/FourEvilDragonsHP/Meshes/DragonTheTerrorBringer/DragonTheTerrorBringerSK.DragonTheTerrorBringerSK'"));
	if (DragonTMesh.Succeeded())
	{
		BossMesh->SetSkeletalMesh(DragonTMesh.Object);
	}

	// Load DragonT-specific animations from DragonTheTerrorBringer folder
	static ConstructorHelpers::FObjectFinder<UAnimSequence> IdleAnim(TEXT("AnimSequence'/Game/FourEvilDragonsHP/Animations/DragonTheTerrorBringer/FlyIdleAnim.FlyIdleAnim'"));
	if (IdleAnim.Succeeded())
	{
		IdleAnimation = IdleAnim.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimSequence> AttackAnim(TEXT("AnimSequence'/Game/FourEvilDragonsHP/Animations/DragonTheTerrorBringer/AttackFlameAnim.AttackFlameAnim'"));
	if (AttackAnim.Succeeded())
	{
		AttackAnimation = AttackAnim.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimSequence> DeathAnim(TEXT("AnimSequence'/Game/FourEvilDragonsHP/Animations/DragonTheTerrorBringer/DieAnim.DieAnim'"));
	if (DeathAnim.Succeeded())
	{
		DeathAnimation = DeathAnim.Object;
	}
}


