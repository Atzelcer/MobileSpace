// Fill out your copyright notice in the Description page of Project Settings.

#include "MegaPortal.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AventuraManager.h"
#include "MobileSpacePawn.h"
#include "HUDmain.h"

AMegaPortal::AMegaPortal()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));
	CollisionComponent->InitCapsuleSize(100.f, 850.f);
	CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	RootComponent = CollisionComponent;

	ParticlePortalComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticlePortalComponent"));
	ParticlePortalComponent->SetupAttachment(RootComponent);
	ParticlePortalComponent->bAutoActivate = false;
	ParticlePortalComponent->SetRelativeScale3D(FVector(3.f, 3.f, 3.f));
}

void AMegaPortal::BeginPlay()
{
	Super::BeginPlay();

	AActor* FoundManager = UGameplayStatics::GetActorOfClass(GetWorld(), AAventuraManager::StaticClass());
	if (FoundManager)
		AventuraManagerPor = Cast<AAventuraManager>(FoundManager);

	CargarParticleSystems();
	EscogerEfectoAleatorio();
}

void AMegaPortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMegaPortal::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor && OtherActor->IsA(AMobileSpacePawn::StaticClass()))
	{
		IrAlSiguienteNivelConTransicion();
		Destroy();
	}
}

void AMegaPortal::CargarParticleSystems()
{
	auto Cargar = [](const TCHAR* Path) -> UParticleSystem*
		{
			return LoadObject<UParticleSystem>(nullptr, Path);
		};

	ParticleSystems = {
		Cargar(TEXT("ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_aquaStorm.P_ky_aquaStorm'")),
		Cargar(TEXT("ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_darkStorm.P_ky_darkStorm'")),
		Cargar(TEXT("ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_fireStorm.P_ky_fireStorm'")),
		Cargar(TEXT("ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_storm.P_ky_storm'")),
		Cargar(TEXT("ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_thunderStorm.P_ky_thunderStorm'"))
	};
}

void AMegaPortal::EscogerEfectoAleatorio()
{
	if (ParticleSystems.Num() == 0) return;

	int32 Index = FMath::RandRange(0, ParticleSystems.Num() - 1);
	UParticleSystem* EfectoSeleccionado = ParticleSystems[Index];

	if (EfectoSeleccionado && ParticlePortalComponent)
	{
		ParticlePortalComponent->SetTemplate(EfectoSeleccionado);
		ParticlePortalComponent->SetVisibility(true);
		ParticlePortalComponent->SetRelativeScale3D(FVector(2.5f, 2.5f, 2.5f));
		ParticlePortalComponent->SetHiddenInGame(false);
		ParticlePortalComponent->Activate(true);
	}
}

void AMegaPortal::IrAlSiguienteNivelConTransicion()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC) return;

	AHUDmain* HUD = Cast<AHUDmain>(PC->GetHUD());
	if (HUD)
	{
		HUD->MostrarPantallaCarga2();
		AventuraManagerPor->ActivarEfectoSonidoPantallaCarga(true);
	}

	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle_SiguienteNivel,
		[this]()
		{
			if (AventuraManagerPor)
			{
				AventuraManagerPor->SiguienteNivel();
			}
		},
		5.0f, false);
}