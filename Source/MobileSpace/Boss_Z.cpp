// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss_Z.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Animation/AnimInstance.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "MobileSpaceProjectile.h"


// Sets default values
ABoss_Z::ABoss_Z()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//mesh
	BossMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BossMesh"));
	BossMesh->SetupAttachment(RootComponent);
	
	ShipCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ShipCollision"));
	ShipCollision->SetupAttachment(RootComponent);
	ShipCollision->SetBoxExtent(FVector(300.f, 300.f, 300.f));
	
	ShipCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ShipCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	 

	
}

// Called when the game starts or when spawned
void ABoss_Z::BeginPlay()
{
	Super::BeginPlay();
	// Initialize health
	CurrentHealth = MaxHealth;

	// Bind overlap event
	ShipCollision->OnComponentBeginOverlap.AddDynamic(this, &ABoss_Z::OnBossHit);

	// Start spawn sequence
	SpawnSequence();

	
}

// Called every frame
void ABoss_Z::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABoss_Z::SpawnSequence()
{
	// Spawn particle effect
	if (SpawnParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			SpawnParticle,
			GetActorLocation(),
			GetActorRotation(),
			FVector(1.f, 1.f, 1.f),
			true
		);
	}

}

void ABoss_Z::DeathSequence()
{
	if (bIsDead) return;

	bIsDead = true;

	// Death particle
	if (DeathParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			DeathParticle,
			GetActorLocation(),
			GetActorRotation(),
			FVector(1.f, 1.f, 1.f),
			true
		);
	}
	Destroy();
}


void ABoss_Z::OnBossHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if (bIsDead || !OtherActor) return;

	// Check if hit by projectile
	AMobileSpaceProjectile* Projectile = Cast<AMobileSpaceProjectile>(OtherActor);
	if (Projectile)
	{
		CurrentHealth -= 10.f;

		UE_LOG(LogTemp, Warning, TEXT("Boss hit! Health: %f"), CurrentHealth);

		// Check death
		if (CurrentHealth <= 0.f)
		{
			DeathSequence();
		}

		Projectile->Destroy();
	}
	
}


