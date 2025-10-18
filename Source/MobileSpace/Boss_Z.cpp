// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss_Z.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "MobileSpaceProjectile.h"


// Sets default values
ABoss_Z::ABoss_Z()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BossMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BossMesh"));
	BossMesh->SetupAttachment(RootComponent);

	ShipCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ShipCollision"));
	ShipCollision->SetupAttachment(RootComponent);
	ShipCollision->SetBoxExtent(FVector(300.f, 300.f, 50.f));

	ShipCollision->OnComponentBeginOverlap.AddDynamic(this, &ABoss_Z::OnBossHit);
	ShipCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ShipCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	

}

// Called when the game starts or when spawned
void ABoss_Z::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABoss_Z::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABoss_Z::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABoss_Z::OnBossHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

