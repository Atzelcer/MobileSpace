// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileZero.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AProjectileZero::AProjectileZero()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	RootComponent = ProjectileMesh;

	ProjectileMesh->SetVisibility(false);
	








}

// Called when the game starts or when spawned
void AProjectileZero::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectileZero::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

