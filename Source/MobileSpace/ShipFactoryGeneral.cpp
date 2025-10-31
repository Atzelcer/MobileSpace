// Fill out your copyright notice in the Description page of Project Settings.


#include "ShipFactoryGeneral.h"
#include "Ship_CazadorRojo.h"
#include "Ship_CazadorAzul.h"
#include "Ship_CazadorVerde.h"
#include "Ship_CazadorOmega.h"
#include "Ship_CazadorDelta.h"
#include "Ship_CazadorAlfa.h"

AShip_X* UShipFactoryGeneral::CrearNave(UWorld* World, ENaveTipo Tipo, const FVector& Loc, const FRotator& Rot)
{
    switch (Tipo)
    {
    case ENaveTipo::Roja: return World->SpawnActor<AShip_CazadorRojo>(AShip_CazadorRojo::StaticClass(), Loc, Rot);
    case ENaveTipo::Azul: return World->SpawnActor<AShip_CazadorAzul>(AShip_CazadorAzul::StaticClass(), Loc, Rot);
    case ENaveTipo::Verde: return World->SpawnActor<AShip_CazadorVerde>(AShip_CazadorVerde::StaticClass(), Loc, Rot);
    case ENaveTipo::Omega: return World->SpawnActor<AShip_CazadorOmega>(AShip_CazadorOmega::StaticClass(), Loc, Rot);
    case ENaveTipo::Delta: return World->SpawnActor<AShip_CazadorDelta>(AShip_CazadorDelta::StaticClass(), Loc, Rot);
    case ENaveTipo::Alfa: return World->SpawnActor<AShip_CazadorAlfa>(AShip_CazadorAlfa::StaticClass(), Loc, Rot);
    default: return nullptr;
    }
}
