// Fill out your copyright notice in the Description page of Project Settings.


#include "Ship_X.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "TimerManager.h"
#include "Components/BoxComponent.h"
#include "MobileSpaceProjectile.h"
#include "MoveComponent.h"


// Sets default values
AShip_X::AShip_X()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Create and setup the mesh component
	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	RootComponent = ShipMesh;

	ShipCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ShipCollision"));
	ShipCollision->SetupAttachment(ShipMesh);
	ShipCollision->SetBoxExtent(FVector(100.f, 100.f, 100.f));
	// IMPORTANTE: Enlazar evento de colisión
	ShipCollision->OnComponentBeginOverlap.AddDynamic(this, &AShip_X::OnShipHit);
	
	// ¡CREAR EL COMPONENTE DE MOVIMIENTO CHISTOSO!
	MoveComp = CreateDefaultSubobject<UMoveComponent>(TEXT("MoveComponent"));
	if (MoveComp)
	{
		// Configurar valores por defecto para móvil
		MoveComp->Speed = 400.0f;
		MoveComp->Amplitude = 250.0f;
		MoveComp->Frequency = 1.5f;
		MoveComp->bUseBounds = true;
	}

	// Configurar colisi�n para que genere eventos
	ShipCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ShipCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	
}

// Called when the game starts or when spawned
void AShip_X::BeginPlay()
{
	Super::BeginPlay();
	
	// Iniciar patrón de movimiento por defecto
	if (MoveComp)
	{
		MoveComp->StartPattern(EMovementPattern::Elliptical); // Patrón básico por defecto
	}
}

// Called every frame
void AShip_X::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AShip_X::OnShipHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, TEXT("Ship collision detected!"));
    }

    if (!OtherActor || OtherActor == this)
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Collision with self or null!"));
        }
        return;
    }

    if (GEngine)
    {
        FString ActorName = OtherActor->GetName();
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("Collided with: %s"), *ActorName));
    }

    // Detectar si es el proyectil del jugador
    if (OtherActor->IsA(AMobileSpaceProjectile::StaticClass()))
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("HIT BY PLAYER PROJECTILE! Destroying ship..."));
        }

        // Destruir la nave enemiga
        Destroy();

    }
}

