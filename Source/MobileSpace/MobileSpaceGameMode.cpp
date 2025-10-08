// Copyright Epic Games, Inc. All Rights Reserved.

#include "MobileSpaceGameMode.h"
#include "MobileSpacePawn.h"

AMobileSpaceGameMode::AMobileSpaceGameMode()
{
	// set default pawn class to our character class
	DefaultPawnClass = AMobileSpacePawn::StaticClass();
}

void AMobileSpaceGameMode::BeginPlay()
{
	Super::BeginPlay();

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Mobile Space Game Mode Initialized"));
}

