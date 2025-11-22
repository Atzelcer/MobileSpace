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
	
	

	// Control de fases de ataque del jefe final
	UFUNCTION()
	void SwitchAttackPhase();

	// Estados de ataque del jefe
	bool bEpicPhase = false; // true = épico 3s, false = pausa 6s
	
	// Timer para la fase épica (3 segundos)
	FTimerHandle EpicPhaseTimerHandle;
	
};
