// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileZero.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AProjectileZero::AProjectileZero()
{
 	// Set this actor to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;
	
	// Crear componente de malla
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	RootComponent = ProjectileMesh;
	ProjectileMesh->SetVisibility(false);
	
	// Crear componente de colisión básico
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(ProjectileMesh);
	CollisionBox->SetBoxExtent(FVector(50.f, 50.f, 50.f));
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Overlap);
	
	// Configurar evento de colisión
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AProjectileZero::OnProjectileHit);
	
	// Configuración por defecto
	Damage = 10.0f;
	ProjectileSpeed = 800.0f;
	HitEffectScale = FVector(1.5f, 1.5f, 1.5f);
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

	// Movimiento del proyectil hacia la izquierda (hacia el jugador)
	FVector CurrentLocation = GetActorLocation();
	CurrentLocation.X -= ProjectileSpeed * DeltaTime;
	SetActorLocation(CurrentLocation);

	// Destruir si sale del mapa por la izquierda
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

	// Verificar si el actor es el pawn del jugador usando el mismo patrón que MobileSpacePawn
	if (OtherActor->GetClass()->GetName().Contains("MobileSpacePawn"))
	{
		// Hacer daño al jugador
		DealDamageToPlayer(OtherActor);
		
		// Reproducir efectos de destrucción
		PlayDestructionEffects();
		
		// Destruir el proyectil
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
	
	// El daño real se maneja en MobileSpacePawn::NotifyActorBeginOverlap cuando detecta AProjectileZero
}

void AProjectileZero::PlayDestructionEffects()
{
	// Por ahora, solo log hasta que se agreguen los includes necesarios
	UE_LOG(LogTemp, Warning, TEXT("ProjectileZero destruido en posición: %s"), *GetActorLocation().ToString());
	
	// Las clases hijas pueden sobreescribir para efectos específicos
}

void AProjectileZero::DestroyProjectile()
{
	Destroy();
}

