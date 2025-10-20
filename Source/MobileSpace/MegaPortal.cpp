// Fill out your copyright notice in the Description page of Project Settings.


#include "MegaPortal.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AventuraManager.h"
#include "MobileSpacePawn.h"

// Sets default values
AMegaPortal::AMegaPortal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));
	CollisionComponent->InitCapsuleSize(100.f, 200.f);
	CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	RootComponent = CollisionComponent;
}



// Called when the game starts or when spawned
void AMegaPortal::BeginPlay()
{
	Super::BeginPlay();
	
	AActor* FoundManager = UGameplayStatics::GetActorOfClass(GetWorld(), AAventuraManager::StaticClass());
	if (FoundManager)
	{
		AventuraManagerRef = Cast<AAventuraManager>(FoundManager);
	}
}




// Called every frame
void AMegaPortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void AMegaPortal::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor && OtherActor->IsA(AMobileSpacePawn::StaticClass()))
	{
		if (AventuraManagerRef)
		{
			AventuraManagerRef->SiguienteNivel();
		}
		this->Destroy();
	}
}



void AMegaPortal::EstablecerMalla(UStaticMesh* NuevaMalla)
{
	if (MeshPortal && NuevaMalla)
	{
		MeshPortal->SetStaticMesh(NuevaMalla);
		UE_LOG(LogTemp, Warning, TEXT("Malla del portal establecida manualmente."));
	}
}


void AMegaPortal::EscogerMallaAleatoria()
{
	int32 RandomIndex = FMath::RandRange(1, 3);

	UStaticMesh* MallaSeleccionada = nullptr;

	switch (RandomIndex)
	{
	case 1:
		MallaSeleccionada = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Meshes/M_PortalTipo1.M_PortalTipo1"));
		break;
	case 2:
		MallaSeleccionada = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Meshes/M_PortalTipo2.M_PortalTipo2"));
		break;
	case 3:
		MallaSeleccionada = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Meshes/M_PortalTipo3.M_PortalTipo3"));
		break;
	default:
		break;
	}

	if (MallaSeleccionada)
	{
		MeshPortal->SetStaticMesh(MallaSeleccionada);
		UE_LOG(LogTemp, Warning, TEXT("Malla aleatoria seleccionada: %d"), RandomIndex);
	}
}