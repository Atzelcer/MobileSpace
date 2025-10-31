// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ShipFactoryGeneral.generated.h"
/**
 * 
 */

UENUM(BlueprintType)
enum class ENaveTipo : uint8
{
    Roja,
    Azul,
    Verde,
    Omega,
    Delta,
    Alfa
};

UCLASS()
class MOBILESPACE_API UShipFactoryGeneral : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION()
    class AShip_X* CrearNave(UWorld* World, ENaveTipo Tipo, const FVector& Loc, const FRotator& Rot);
	
};
