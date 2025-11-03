// Fill out your copyright notice in the Description page of Project Settings.


#include "Ship_X.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "TimerManager.h"
#include "Components/BoxComponent.h"
#include "MobileSpaceProjectile.h"
#include "MoveComponent.h"
#include "MegaCapsula.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"

AShip_X::AShip_X()
{
	PrimaryActorTick.bCanEverTick = true;
	
	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	RootComponent = ShipMesh;

	ShipCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ShipCollision"));
	ShipCollision->SetupAttachment(ShipMesh);
	ShipCollision->SetBoxExtent(FVector(350.f, 365.f, 200.f));

	// Configure collision properly
	ShipCollision->OnComponentBeginOverlap.AddDynamic(this, &AShip_X::OnShipHit);
	
	ShipCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ShipCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	MoveComp = CreateDefaultSubobject<UMoveComponent>(TEXT("MoveComp"));

	static ConstructorHelpers::FObjectFinder<USoundBase> ExplosionSound(TEXT("SoundWave'/Game/StarterContent/Audio/Explosion01.Explosion01'"));
	if (ExplosionSound.Succeeded())
		SonidoExplosion = ExplosionSound.Object;


	ProbabilidadSpawnCapsula = 23;
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
		HandleDestruction();
    }
}

void AShip_X::IntentarSpawnCapsula()
{
	UWorld* World = GetWorld();
	if (!World) return;

	int32 NumeroAleatorio = FMath::RandRange(1, 100);

	if (NumeroAleatorio <= ProbabilidadSpawnCapsula)
	{
		const FVector SpawnLocation = GetActorLocation();
		const FRotator SpawnRotation = GetActorRotation();

		World->SpawnActor<AMegaCapsula>(AMegaCapsula::StaticClass(), SpawnLocation, SpawnRotation);
	}
}

void AShip_X::DestruirNave()
{
	if (SonidoExplosion)
	{
		UGameplayStatics::PlaySoundAtLocation(this, SonidoExplosion, GetActorLocation());
	}

	IntentarSpawnCapsula();
	Destroy();
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

	IntentarSpawnCapsula();
	Destroy();
}