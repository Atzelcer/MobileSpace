// Fill out your copyright notice in the Description page of Project Settings.


#include "ShipFactoryGeneral.h"
#include "Ship_CazadorRojo.h"
#include "Ship_CazadorAzul.h"
#include "Ship_CazadorVerde.h"
#include "Ship_CazadorOmega.h"
#include "Ship_CazadorDelta.h"
#include "Ship_CazadorAlfa.h"

AShip_X* UShipFactoryGeneral::CrearNave(UWorld* World, ENaveTipo Tipo, FVector Loc, FRotator Rot)
{
    if (Loc.IsZero())
        Loc = DefaultLocation;
    if (Rot.IsZero())
        Rot = DefaultRotation;

    AShip_X* Ship = nullptr;
    switch (Tipo)
    {
    case ENaveTipo::Roja: Ship = World->SpawnActor<AShip_CazadorRojo>(AShip_CazadorRojo::StaticClass(), Loc, Rot); break;
    case ENaveTipo::Azul: Ship = World->SpawnActor<AShip_CazadorAzul>(AShip_CazadorAzul::StaticClass(), Loc, Rot); break;
    case ENaveTipo::Verde: Ship = World->SpawnActor<AShip_CazadorVerde>(AShip_CazadorVerde::StaticClass(), Loc, Rot); break;
    case ENaveTipo::Omega: Ship = World->SpawnActor<AShip_CazadorOmega>(AShip_CazadorOmega::StaticClass(), Loc, Rot); break;
    case ENaveTipo::Delta: Ship = World->SpawnActor<AShip_CazadorDelta>(AShip_CazadorDelta::StaticClass(), Loc, Rot); break;
    case ENaveTipo::Alfa: Ship = World->SpawnActor<AShip_CazadorAlfa>(AShip_CazadorAlfa::StaticClass(), Loc, Rot); break;
    default: break;
    }
    return Ship;
}
