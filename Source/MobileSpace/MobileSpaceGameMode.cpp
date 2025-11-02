// Copyright Epic Games, Inc. All Rights Reserved.

#include "MobileSpaceGameMode.h"
#include "MobileSpacePawn.h"
#include "Engine/World.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "HUDmain.h"
#include "MegaPortal.h"
#include "AventuraManager.h"

AMobileSpaceGameMode::AMobileSpaceGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	HUDClass = AHUDmain::StaticClass();	
	
	//DefaultPawnClass = AMobileSpacePawn::StaticClass();
}

void AMobileSpaceGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AMobileSpaceGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

