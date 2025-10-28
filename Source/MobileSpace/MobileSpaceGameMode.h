// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MobileSpaceGameMode.generated.h"
/**
 * 
 */
UCLASS(MinimalAPI)
class AMobileSpaceGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMobileSpaceGameMode();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	// TArray to hold the enemy ships in formation
	

	// Fixed camera for top-down view
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class ACameraActor* FixedCamera;

	// Adventure Manager for level control
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Management")
	class AAventuraManager* AventuraManager;

protected:
	
	// Function to setup fixed camera
	UFUNCTION(BlueprintCallable, Category = "Camera")
	void SetupFixedCamera();



};



