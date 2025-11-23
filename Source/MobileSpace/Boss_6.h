// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Boss.h"
#include "Boss_6.generated.h"

/**
 * Jefe 6 - Nave CF2_Shuttle1 (Jefe Final - El más poderoso)
 */
UCLASS()
class MOBILESPACE_API ABoss_6 : public ABoss
{
	GENERATED_BODY()

public:
	ABoss_6();

protected:
	virtual void BeginPlay() override;
	
	

	UFUNCTION()
	void SwitchAttackPhase();

	bool bEpicPhase = false;
	
	FTimerHandle EpicPhaseTimerHandle;
	
};
