// Fill out your copyright notice in the Description page of Project Settings.


#include "Ship_X.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "TimerManager.h"
#include "Components/BoxComponent.h"
#include "MobileSpaceProjectile.h"
#include "MoveComponent.h"

AShip_X::AShip_X()
{
	PrimaryActorTick.bCanEverTick = true;
	
	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	RootComponent = ShipMesh;

	ShipCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ShipCollision"));
	ShipCollision->SetupAttachment(ShipMesh);
	ShipCollision->SetBoxExtent(FVector(100.f, 100.f, 100.f));
	ShipCollision->OnComponentBeginOverlap.AddDynamic(this, &AShip_X::OnShipHit);
	
	ShipCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ShipCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	MoveComp = CreateDefaultSubobject<UMoveComponent>(TEXT("MoveComp"));
	
}

void AShip_X::BeginPlay()
{
	Super::BeginPlay();
	
}

void AShip_X::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AShip_X::OnShipHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

    if (OtherActor->IsA(AMobileSpaceProjectile::StaticClass()))
    {
        Destroy();
    }
}

