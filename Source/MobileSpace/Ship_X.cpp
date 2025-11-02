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

	ShipCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ShipCollision->SetCollisionResponseToAllChannels(ECR_Block);
	ShipCollision->SetCollisionObjectType(ECC_Pawn); 

	ShipCollision->OnComponentHit.AddDynamic(this, &AShip_X::OnShipHit);

	MoveComp = CreateDefaultSubobject<UMoveComponent>(TEXT("MoveComp"));


	static ConstructorHelpers::FObjectFinder<USoundBase> SoundAsset(TEXT("SoundWave'/Game/StarterContent/Audio/Explosion01.Explosion01'"));
	if (SoundAsset.Succeeded()) {
		DestructionSound = SoundAsset.Object;
	}
}

void AShip_X::BeginPlay()
{
	Super::BeginPlay();
	
}

void AShip_X::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AShip_X::OnShipHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor->IsA(AMobileSpaceProjectile::StaticClass()))
	{
		Destroy();
	}
}

void AShip_X::HandleDestruction()
{
	if (DestructionEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DestructionEffect, GetActorLocation(), GetActorRotation(), FVector(3.f, 3.f, 3.f));
	}

	if (DestructionSound) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), DestructionSound, GetActorLocation());
	}

	
	Destroy();
}

