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
	HUDClass = AHUDmain::StaticClass();	
}

void AMobileSpaceGameMode::BeginPlay()
{
	Super::BeginPlay();

	SetupFixedCamera();
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
			//UE_LOG(LogTemp, Warning, TEXT("Fixed camera created and set as view target!"));
		}
	}
}



