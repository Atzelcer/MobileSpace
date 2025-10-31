// Fill out your copyright notice in the Description page of Project Settings.

#include "MegaPortal.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "AventuraManager.h"
#include "MobileSpacePawn.h"

AMegaPortal::AMegaPortal()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));
	CollisionComponent->InitCapsuleSize(100.f, 200.f);
	CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	RootComponent = CollisionComponent;

	NiagaraPortalComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraPortalComponent"));
	NiagaraPortalComponent->SetupAttachment(RootComponent);
}

void AMegaPortal::BeginPlay()
{
	Super::BeginPlay();

	//AActor* FoundManager = UGameplayStatics::GetActorOfClass(GetWorld(), AAventuraManager::StaticClass());
	//if (FoundManager)
	//	AventuraManagerPor = Cast<AAventuraManager>(FoundManager);

	CargarNiagaraSystems();
	EscogerEfectoAleatorio();
}

void AMegaPortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMegaPortal::NotifyActorBeginOverlap(AActor* OtherActor)
{
	//if (OtherActor && OtherActor->IsA(AMobileSpacePawn::StaticClass()))
	//{
	//	if (AventuraManagerPor)
	//		AventuraManagerPor->SiguienteNivel();

	//	Destroy();
	//}
}

void AMegaPortal::CargarNiagaraSystems()
{
	auto Cargar = [](const TCHAR* Path) -> UNiagaraSystem*
		{
			return LoadObject<UNiagaraSystem>(nullptr, Path);
		};

	NiagaraSystems = {
		Cargar(TEXT("/Game/Portals_VFXPack/Particles/P_Fire_EllipsePortal.P_Fire_EllipsePortal")),
		Cargar(TEXT("/Game/Portals_VFXPack/Particles/P_Fog_CapturePortal.P_Fog_CapturePortal")),
		Cargar(TEXT("/Game/Portals_VFXPack/Particles/P_Futuristic_CapturePortal.P_Futuristic_CapturePortal")),
		Cargar(TEXT("/Game/Portals_VFXPack/Particles/P_Level_LowPolyPortal_Blue.P_Level_LowPolyPortal_Blue")),
		Cargar(TEXT("/Game/Portals_VFXPack/Particles/P_Level_LowPolyPortal_Yellow.P_Level_LowPolyPortal_Yellow")),
		Cargar(TEXT("/Game/Portals_VFXPack/Particles/P_LowPoly_CapturePortal_01.P_LowPoly_CapturePortal_01")),
		Cargar(TEXT("/Game/Portals_VFXPack/Particles/P_LowPoly_CapturePortal_02.P_LowPoly_CapturePortal_02"))
	};
}

void AMegaPortal::EscogerEfectoAleatorio()
{
	if (NiagaraSystems.Num() == 0) return;

	int32 Index = FMath::RandRange(0, NiagaraSystems.Num() - 1);
	UNiagaraSystem* EfectoSeleccionado = NiagaraSystems[Index];

	if (EfectoSeleccionado && NiagaraPortalComponent)
	{
		NiagaraPortalComponent->SetAsset(EfectoSeleccionado);
		NiagaraPortalComponent->Activate(true);
		UE_LOG(LogTemp, Warning, TEXT("Efecto Niagara seleccionado: %d"), Index);
	}
}


