// Copyright Epic Games, Inc. All Rights Reserved.

#include "MobileSpaceGameMode.h"
#include "MobileSpacePawn.h"
#include "Engine/World.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "HUDmain.h"
#include "MegaPortal.h"

AMobileSpaceGameMode::AMobileSpaceGameMode()
{
	// set default pawn class to our character class
	DefaultPawnClass = AMobileSpacePawn::StaticClass();
	HUDClass = AHUDmain::StaticClass();
	bUseSeamlessTravel = true;

}

void AMobileSpaceGameMode::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::SetGamePaused(GetWorld(), true);

	// También puedes ocultar el cursor si quieres
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = true;
		PlayerController->SetInputMode(FInputModeUIOnly());
	}
	// Setup fixed camera
	SetupFixedCamera();
	UWorld* World = GetWorld();
	if (World)
	{
		FVector PortalLocation(1610.0f, -250.0f, 210.0f);
		FRotator PortalRotation(0.0f, 0.0f, 0.0f);
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AMegaPortal* Portal = World->SpawnActor<AMegaPortal>(AMegaPortal::StaticClass(), PortalLocation, PortalRotation, SpawnParams);
		if (Portal)
		{
			UE_LOG(LogTemp, Warning, TEXT("MegaPortal spawneado correctamente en (1610, -250, 160)"));
		}
	}
}

void AMobileSpaceGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void AMobileSpaceGameMode::SetupFixedCamera()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	// Create a fixed camera actor
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Position for top-down view (like classic arcade games)
	//FVector CameraLocation = FVector(-163190.0f, 6103422.0f, 9825.0f); // High above the map
	FVector CameraLocation = FVector(0.0f, 0.0f, 2000.0f); // High above the map
	FRotator CameraRotation = FRotator(-90.0f, 0.0f, 0.0f); // Looking straight down

	FixedCamera = World->SpawnActor<ACameraActor>(CameraLocation, CameraRotation, SpawnParams);

	if (FixedCamera)
	{
		// Configure camera for orthographic view (optional - for true 2D feel)
		UCameraComponent* CameraComponent = FixedCamera->GetCameraComponent();
		if (CameraComponent)
		{
			// Uncomment next line for pure 2D orthographic view
			// CameraComponent->SetProjectionMode(ECameraProjectionMode::Orthographic);
			// CameraComponent->SetOrthoWidth(3000.0f);

			// For perspective but wide field of view
			CameraComponent->SetFieldOfView(120.0f); // Wide angle for better view
		}

		// Set this camera as the view target for all players
		APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
		if (PC)
		{
			PC->SetViewTarget(FixedCamera);
			UE_LOG(LogTemp, Warning, TEXT("Fixed camera created and set as view target!"));
		}
	}
}