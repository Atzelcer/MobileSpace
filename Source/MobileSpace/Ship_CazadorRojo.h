// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ship_X.h"
#include "AtackComponent.h"
#include "Ship_CazadorRojo.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESPACE_API AShip_CazadorRojo : public AShip_X
{
	GENERATED_BODY()

public:

	AShip_CazadorRojo();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAtackComponent* AttackComp;

	// Patrón de ataque que usará este enemigo
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	EAtackPattern AttackPattern = EAtackPattern::Spread;

	// Tiempo entre disparos
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float TimeBetweenShots = 3.0f;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	FTimerHandle FireTimerHandle;
	void AutoFire();
};
