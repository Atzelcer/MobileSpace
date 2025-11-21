// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MobEnums.generated.h"

UENUM(BlueprintType)
enum class EAttackPattern : uint8
{
    None,
    Single,
    Spread,
    Burst,
    TripleArc,
    SniperShot,
    Circular
};

UENUM(BlueprintType)
enum class EArcadeMovement : uint8
{
    None,
    Wave,
    SEntry,
    RadialBurst,
    WavyWave,
    ExpandingCircle,
    BossMajesticArc,
    SuperSequence
};

UENUM(BlueprintType)
enum class ENaveTipo : uint8
{
    Roja,
    Azul,
    Verde,
    Delta,
    Alfa,
    Omega
};

UENUM(BlueprintType)
enum class EShipRole : uint8
{
    None,
    Normal,
    Perseguidor,
    Francotirador,
    Kamikaze,
    Jefe,
    Soporte,
    Elite,
    Rafaga,
    Tanque
};

UENUM(BlueprintType)
enum class EFormationType : uint8
{
    Fila,
    Columna,
    Cuadricula,
    VShape,
    Swarm,
    Diagonal,
    Escalon,
    RandomScatter
};