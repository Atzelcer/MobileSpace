// Fill out your copyright notice in the Description page of Project Settings.

#include "DragonR_Boss_Z.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

ADragonR_Boss_Z::ADragonR_Boss_Z()
{
	PrimaryActorTick.bCanEverTick = true;

	// Setup DragonR-specific mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> DragonRMesh(TEXT("SkeletalMesh'/Game/FourEvilDragonsHP/Meshes/DragonTheUsurper/DragonTheUsurperSK.DragonTheUsurperSK'"));
	if (DragonRMesh.Succeeded())
	{
		BossMesh->SetSkeletalMesh(DragonRMesh.Object);
	}

	// Load DragonR-specific animations
	static ConstructorHelpers::FObjectFinder<UAnimSequence> IdleAnim(TEXT("AnimSequence'/Game/FourEvilDragonsHP/Animations/DragonTheUsurper/FlyIdleAnim.FlyIdleAnim'"));
	if (IdleAnim.Succeeded())
	{
		IdleAnimation = IdleAnim.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimSequence> AttackAnim(TEXT("AnimSequence'/Game/FourEvilDragonsHP/Animations/DragonTheUsurper/AttackFlameAnim.AttackFlameAnim'"));
	if (AttackAnim.Succeeded())
	{
		AttackAnimation = AttackAnim.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimSequence> DeathAnim(TEXT("AnimSequence'/Game/FourEvilDragonsHP/Animations/DragonTheUsurper/DieAnim.DieAnim'"));
	if (DeathAnim.Succeeded())
	{
		DeathAnimation = DeathAnim.Object;
	}
}


