// Fill out your copyright notice in the Description page of Project Settings.


#include "FacadeGameManager.h"
#include "AventuraManager.h"
#include "MultiplayerManager.h"
#include "Engine/World.h"

// Sets default values
AFacadeGameManager::AFacadeGameManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFacadeGameManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFacadeGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFacadeGameManager::IniciarModoAventura()
{
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
	UWorld* World = GetWorld();
	if (World && !MultiplayerManagerRef)
	{
		FActorSpawnParameters Params;
		Params.Owner = this;
		MultiplayerManagerRef = World->SpawnActor<AMultiplayerManager>(AMultiplayerManager::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);
	}
}

