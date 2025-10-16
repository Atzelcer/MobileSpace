// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss_Z.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
ABoss_Z::ABoss_Z()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BossMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BossMesh"));
	BossMesh->SetupAttachment(RootComponent);

	

}

// Called when the game starts or when spawned
void ABoss_Z::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABoss_Z::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABoss_Z::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

