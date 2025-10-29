// Fill out your copyright notice in the Description page of Project Settings.

#include "ShipMultijugador.h"
#include "ProjectileMultijugador.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundBase.h"
#include "UObject/ConstructorHelpers.h"

AShipMultijugador::AShipMultijugador()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);

	// === MALLA PRINCIPAL ===
	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	RootComponent = ShipMesh;
	ShipMesh->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	ShipMesh->SetIsReplicated(true);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/StarSparrow/Meshes/Examples/SM_StarSparrow19.SM_StarSparrow19'"));
	if (MeshAsset.Succeeded())
	{
		ShipMesh->SetStaticMesh(MeshAsset.Object);
		ShipMesh->SetRelativeScale3D(FVector(0.3f));
	}

	// === PARTÍCULAS DE COLA ===
	ParticleTrail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleTrail"));
	ParticleTrail->SetupAttachment(ShipMesh);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("ParticleSystem'/Game/MagicProjectilesVol2/Particles/Projectiles/P_Projectile_ElectricBall01_Yellow.P_Projectile_ElectricBall01_Yellow'"));
	if (ParticleAsset.Succeeded())
	{
		ParticleTrail->SetTemplate(ParticleAsset.Object);
		ParticleTrail->SetRelativeLocation(FVector(-500.f, 0.f, 0.f));
		ParticleTrail->SetRelativeScale3D(FVector(2.f));
	}

	// === SONIDO DE DISPARO ===
	static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("SoundWave'/Game/Free_Sounds_Pack/wav/Sci-Fi_Gun_1-1.Sci-Fi_Gun_1-1'"));
	if (FireAudio.Succeeded())
	{
		FireSound = FireAudio.Object;
	}

	// === ESTADÍSTICAS ===
	VidaMaxima = 100.f;
	VidaActual = 100.f;
	VelocidadActual = 600.f;
	CantidadMisiles = 3;
	CantidadEscudos = 2;
	LimiteDisparo = 0.5f;
	bCanFire = true;
}

void AShipMultijugador::BeginPlay()
{
	Super::BeginPlay();
}

void AShipMultijugador::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AShipMultijugador::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AShipMultijugador::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShipMultijugador::MoveRight);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AShipMultijugador::FireShot);
}

void AShipMultijugador::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector(), Value);
}

void AShipMultijugador::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector(), Value);
}

void AShipMultijugador::FireShot()
{
	if (!bCanFire) return;
	bCanFire = false;
	Server_FireShot();
	GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AShipMultijugador::ResetFire, LimiteDisparo, false);
}

void AShipMultijugador::Server_FireShot_Implementation()
{
	FVector SpawnLoc = GetActorLocation() + GetActorForwardVector() * 100.f;
	FRotator SpawnRot = GetActorRotation();
	GetWorld()->SpawnActor<AProjectileMultijugador>(AProjectileMultijugador::StaticClass(), SpawnLoc, SpawnRot);
}

void AShipMultijugador::ResetFire()
{
	bCanFire = true;
}

void AShipMultijugador::ActualizarHUD() {}

void AShipMultijugador::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AShipMultijugador, VidaActual);
	DOREPLIFETIME(AShipMultijugador, VelocidadActual);
}
