// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ship_X.h"
#include "MoveComponent.h"
#include "AtackComponent.h"
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UMoveComponent* MoveComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAtackComponent* AttackComp;


	// Patrón de ataque que usará este enemigo
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	EAtackPattern AttackPattern = EAtackPattern::Single;

	// Tiempo entre disparos
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float TimeBetweenShots = 2.0f;

protected:
	// Override movement behavior for this specific ship type
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

private:
	FTimerHandle FireTimerHandle;
	void AutoFire();
	




	
};
