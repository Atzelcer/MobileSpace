// Fill out your copyright notice in the Description page of Project Settings.


#include "Ship_X.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AShip_X::AShip_X()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	ShipMesh->SetupAttachment(RootComponent);



}

// Called when the game starts or when spawned
void AShip_X::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShip_X::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

