// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MobEnums.h"
#include "ShipFactoryGeneral.generated.h"

UCLASS()
class MOBILESPACE_API UShipFactoryGeneral : public UObject
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector DefaultLocation = FVector(1000.f, 0.f, 300.f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FRotator DefaultRotation = FRotator::ZeroRotator;

    UFUNCTION()
    class AShip_X* CrearNave(
        UWorld* World,
        ENaveTipo Tipo,
        FVector Loc = FVector::ZeroVector,
        FRotator Rot = FRotator::ZeroRotator
    );
};
