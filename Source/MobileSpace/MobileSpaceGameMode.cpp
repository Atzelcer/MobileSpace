// Copyright Epic Games, Inc. All Rights Reserved.

#include "MobileSpaceGameMode.h"
#include "MobileSpacePawn.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "HUDmain.h"

AMobileSpaceGameMode::AMobileSpaceGameMode()
{
	// set default pawn class to our character class
	DefaultPawnClass = AMobileSpacePawn::StaticClass();

	// use our custom HUD class
	HUDClass = AHUDmain::StaticClass();
}

void AMobileSpaceGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Pausar el juego al iniciar
	UGameplayStatics::SetGamePaused(GetWorld(), true);

	// También puedes ocultar el cursor si quieres
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = true;
		PlayerController->SetInputMode(FInputModeUIOnly());
	}
}

