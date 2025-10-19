// Fill out your copyright notice in the Description page of Project Settings.


#include "FacadeGameManager.h"

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
}

void AFacadeGameManager::IniciarModoMultijugador()
{
}

