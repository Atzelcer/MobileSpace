// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ship_X.h"
#include "Ship_CazadorVerde.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESPACE_API AShip_CazadorVerde : public AShip_X
{
	GENERATED_BODY()
	
public:

	AShip_CazadorVerde();

protected:
	// Override movement behavior for this specific ship type
	virtual void BeginPlay() override;
	virtual void UpdateMovement(float DeltaTime) override;
	virtual void StartAttackPattern() override;

	// Timer to change movement patterns
	FTimerHandle PatternChangeTimer;

public:
	// Functions to change movement patterns
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void CycleMovementPattern();

private:
	// Simple attack timing
	float AttackTime = 0.0f;
	
};
