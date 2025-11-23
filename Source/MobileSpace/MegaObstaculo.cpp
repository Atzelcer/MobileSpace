// Fill out your copyright notice in the Description page of Project Settings.

#include "MegaObstaculo.h"
#include "Kismet/GameplayStatics.h"
#include "MobileSpacePawn.h"

AMegaObstaculo::AMegaObstaculo()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	RootComponent = CollisionComp;
	CollisionComp->SetBoxExtent(FVector(250.f, 250.f, 1150.f));
	CollisionComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	static ConstructorHelpers::FObjectFinder<USoundWave> Sonido(TEXT("SoundWave'/Game/Free_Sounds_Pack/wav/Whoosh_4-1.Whoosh_4-1'"));
	if (Sonido.Succeeded())
		SonidoDestruccion = Sonido.Object;

	VelocidadMovimiento = 1000.f;
}

void AMegaObstaculo::BeginPlay()
{
	Super::BeginPlay();
	AsignarMallaAleatoria();
}

void AMegaObstaculo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoverObstaculo(DeltaTime);
	VerificarDestruccion();
}


void AMegaObstaculo::AsignarMallaAleatoria()
{
	TipoObstaculo = FMath::RandRange(1, 4);
	FString Path;

	switch (TipoObstaculo)
	{
	case 1: Path = TEXT("StaticMesh'/Game/CF2Shuttle/Meshes/Asteroids/SM_Asteroid_A.SM_Asteroid_A'"); break;
	case 2: Path = TEXT("StaticMesh'/Game/CF2Shuttle/Meshes/Asteroids/SM_Asteroid_A.SM_Asteroid_A'"); break;
	case 3: Path = TEXT("StaticMesh'/Game/CF2Shuttle/Meshes/Asteroids/SM_Asteroid_B.SM_Asteroid_B'"); break;
	case 4: Path = TEXT("StaticMesh'/Game/CF2Shuttle/Meshes/Asteroids/SM_Asteroid_B.SM_Asteroid_B'"); break;
	default: return;
	}

	UStaticMesh* Malla = LoadObject<UStaticMesh>(nullptr, *Path);
	if (Malla)
	{
		MeshComp->SetStaticMesh(Malla);
		
	}
}

void AMegaObstaculo::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	AMobileSpacePawn* Nave = Cast<AMobileSpacePawn>(OtherActor);
	if (Nave)
	{
		Nave->HacerDanio();
		DestruirObstaculo();
	}

	AMobileSpaceProjectile* Proyectil = Cast<AMobileSpaceProjectile>(OtherActor);
	if (Proyectil)
	{
		DestruirObstaculo();
		Proyectil->Destroy();
		return;
	}
}

void AMegaObstaculo::DestruirObstaculo()
{
	if (SonidoDestruccion)
		UGameplayStatics::PlaySoundAtLocation(this, SonidoDestruccion, GetActorLocation());

	Destroy();
}

void AMegaObstaculo::MoverObstaculo(float DeltaTime)
{
	FVector NuevaPos = GetActorLocation();
	
	if (bMovimientoDiagonal)
	{
		// Movimiento diagonal usando la dirección configurada
		NuevaPos += DireccionMovimiento * VelocidadMovimiento * DeltaTime;
	}
	else
	{
		// Movimiento tradicional solo en X (hacia la izquierda)
		NuevaPos.X -= VelocidadMovimiento * DeltaTime;
	}
	
	SetActorLocation(NuevaPos);
}

void AMegaObstaculo::VerificarDestruccion()
{
	FVector Pos = GetActorLocation();
	
	// Verificar si está fuera de los límites del mapa
	if (Pos.X <= -1500.0f || Pos.X >= 2000.0f || 
	    Pos.Y <= -3500.0f || Pos.Y >= 3500.0f ||
	    Pos.Z <= -500.0f || Pos.Z >= 1000.0f)
	{
		Destroy();
	}
}

void AMegaObstaculo::SetVelocidadMovimiento(float NuevaVelocidad)
{
	VelocidadMovimiento = NuevaVelocidad;
}
