// Fill out your copyright notice in the Description page of Project Settings.


#include "FacadeGameManager.h"
#include "AventuraManager.h"
#include "MultiplayerManager.h"
#include "Engine/World.h"

AFacadeGameManager::AFacadeGameManager()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AFacadeGameManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFacadeGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFacadeGameManager::IniciarModoAventura()
{
	if (AventuraManagerRef) return;

	UWorld* World = GetWorld();
	if (World && !AventuraManagerRef)
	{
		FActorSpawnParameters Params;
		Params.Owner = this;
		AventuraManagerRef = World->SpawnActor<AAventuraManager>(AAventuraManager::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);
	}
}

void AFacadeGameManager::IniciarModoMultijugador()
{
	if (MultiplayerManagerRef) return;

	UWorld* World = GetWorld();
	if (World && !MultiplayerManagerRef)
	{
		FActorSpawnParameters Params;
		Params.Owner = this;
		MultiplayerManagerRef = World->SpawnActor<AMultiplayerManager>(AMultiplayerManager::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);
	}
}

