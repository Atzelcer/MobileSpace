// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ship_X.h"
#include "AtackComponent.h"
#include "Ship_CazadorAzul.generated.h"

UCLASS()
class MOBILESPACE_API AShip_CazadorAzul : public AShip_X
{
	GENERATED_BODY()

public:

	AShip_CazadorAzul();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAtackComponent* AttackComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	EAtackPattern AttackPattern = EAtackPattern::Single;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float TimeBetweenShots = 1.8f;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	FTimerHandle FireTimerHandle;
	void AutoFire();

public:
	virtual void DestruirNave() override;
};
