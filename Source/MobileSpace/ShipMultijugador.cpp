// Fill out your copyright notice in the Description page of Project Settings.

#include "ShipMultijugador.h"
#include "ProjectileMultijugador.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Net/UnrealNetwork.h"
#include "WidgetOnGameMulti.h"
#include "Blueprint/UserWidget.h"

AShipMultijugador::AShipMultijugador()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMeshAsset(TEXT("/Game/StarSparrow/Meshes/Examples/SM_StarSparrow19.SM_StarSparrow19"));
	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	SetRootComponent(ShipMesh);
	if (ShipMeshAsset.Succeeded())
	{
		ShipMesh->SetStaticMesh(ShipMeshAsset.Object);
		ShipMesh->SetRelativeScale3D(FVector(0.3f));
	}
	ShipMesh->SetIsReplicated(true);
	SetReplicateMovement(true);

	ParticleTrail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TrailFX"));
	ParticleTrail->SetupAttachment(ShipMesh);

	VidaMaxima = 100.f;
	VidaActual = VidaMaxima;
	VelocidadActual = 1200.f;
	CantidadMisiles = 5;
	CantidadEscudos = 3;
	LimiteDisparo = 1.0f;
	bCanFire = true;
}

void AShipMultijugador::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocallyControlled())
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		if (PC && WidgetMultiClass)
		{
			WidgetMultiInstance = CreateWidget<UWidgetOnGameMulti>(PC, WidgetMultiClass);
			if (WidgetMultiInstance)
			{
				WidgetMultiInstance->AddToViewport(5);
				ActualizarHUD();
			}
		}
	}
}

void AShipMultijugador::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsLocallyControlled() && WidgetMultiInstance)
	{
		ActualizarHUD();
	}
}

void AShipMultijugador::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AShipMultijugador::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShipMultijugador::MoveRight);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AShipMultijugador::FireShot);
}

void AShipMultijugador::ActualizarHUD()
{
	if (!WidgetMultiInstance) return;
	WidgetMultiInstance->ActualizarVida(VidaActual, VidaMaxima);
	WidgetMultiInstance->ActualizarLimiteDisparo(LimiteDisparo, 1.0f);
	WidgetMultiInstance->ActualizarVelocidad(VelocidadActual);
	WidgetMultiInstance->ActualizarMisiles(CantidadMisiles);
	WidgetMultiInstance->ActualizarEscudo(CantidadEscudos);
}

void AShipMultijugador::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		AddActorWorldOffset(FVector(Value * VelocidadActual * GetWorld()->DeltaTimeSeconds, 0.f, 0.f), true);
	}
}

void AShipMultijugador::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		AddActorWorldOffset(FVector(0.f, Value * VelocidadActual * GetWorld()->DeltaTimeSeconds, 0.f), true);
	}
}

void AShipMultijugador::FireShot()
{
	if (!bCanFire) return;
	Server_FireShot();
	bCanFire = false;
	GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &AShipMultijugador::ResetFire, 0.25f);
}

void AShipMultijugador::Server_FireShot_Implementation()
{
	FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 90.f;
	FRotator SpawnRotation = GetActorRotation();
	UWorld* World = GetWorld();
	if (World)
	{
		World->SpawnActor<AProjectileMultijugador>(AProjectileMultijugador::StaticClass(), SpawnLocation, SpawnRotation);
	}
}

void AShipMultijugador::ResetFire()
{
	bCanFire = true;
}

void AShipMultijugador::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AShipMultijugador, VidaActual);
	DOREPLIFETIME(AShipMultijugador, VelocidadActual);
}
