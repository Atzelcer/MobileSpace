// Copyright Epic Games, Inc. All Rights Reserved.

#include "MobileSpaceGameMode.h"
#include "MobileSpacePawn.h"

AMobileSpaceGameMode::AMobileSpaceGameMode()
{
	// set default pawn class to our character class
	DefaultPawnClass = AMobileSpacePawn::StaticClass();
}

