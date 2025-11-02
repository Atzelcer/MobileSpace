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
	CollisionComponent->InitCapsuleSize(100.f, 200.f);
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
		Cargar(TEXT("ParticleSystem'/Game/VE_PowerUps/Particles/PS_06_Y1.PS_06_Y1'")),
		Cargar(TEXT("ParticleSystem'/Game/VE_PowerUps/Particles/PS_06_R1.PS_06_R1'")),
		Cargar(TEXT("ParticleSystem'/Game/VE_PowerUps/Particles/PS_06_B.PS_06_B'")),
		Cargar(TEXT("ParticleSystem'/Game/VE_PowerUps/Particles/PS_27_R.PS_27_R'"))
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
		HUD->MostrarPantallaCarga();
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