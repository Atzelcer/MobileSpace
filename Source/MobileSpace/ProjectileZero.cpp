// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileZero.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AProjectileZero::AProjectileZero()
{
	PrimaryActorTick.bCanEverTick = true;
	
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	RootComponent = ProjectileMesh;
	ProjectileMesh->SetVisibility(false);
	
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(ProjectileMesh);
	CollisionBox->SetBoxExtent(FVector(50.f, 50.f, 50.f));
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Overlap);
	
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AProjectileZero::OnProjectileHit);
	
	Damage = 10.0f;
	ProjectileSpeed = 800.0f;
	HitEffectScale = FVector(1.5f, 1.5f, 1.5f);
}

void AProjectileZero::BeginPlay()
{
	Super::BeginPlay();
	
}

void AProjectileZero::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector CurrentLocation = GetActorLocation();
	CurrentLocation.X -= ProjectileSpeed * DeltaTime;
	SetActorLocation(CurrentLocation);

	if (CurrentLocation.X < -2000.0f)
	{
		Destroy();
	}
}

void AProjectileZero::OnProjectileHit(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!OtherActor) return;

	if (OtherActor->GetClass()->GetName().Contains("MobileSpacePawn"))
	{
		DealDamageToPlayer(OtherActor);
		
		PlayDestructionEffects();
		
		DestroyProjectile();
	}
	else if (OtherActor->GetClass()->GetName().Contains("MobileSpaceProjectile"))
	{
		PlayDestructionEffects();
		DestroyProjectile();
	}
}

void AProjectileZero::DealDamageToPlayer(AActor* PlayerActor)
{
	if (!PlayerActor) return;
	
	// Log del daño recibido
	UE_LOG(LogTemp, Warning, TEXT("¡Jugador (%s) recibió %.1f de daño por ProjectileZero!"), 
		   *PlayerActor->GetName(), Damage);
	
}

void AProjectileZero::PlayDestructionEffects()
{
	UE_LOG(LogTemp, Warning, TEXT("ProjectileZero destruido en posición: %s"), *GetActorLocation().ToString());
	
}

void AProjectileZero::DestroyProjectile()
{
	Destroy();
}

