// Fill out your copyright notice in the Description page of Project Settings.

#include "MegaObstaculo.h"
#include "Kismet/GameplayStatics.h"
#include "MobileSpacePawn.h"

AMegaObstaculo::AMegaObstaculo()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	RootComponent = CollisionComp;
	CollisionComp->SetBoxExtent(FVector(550.f, 550.f, 550.f));
	CollisionComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	static ConstructorHelpers::FObjectFinder<USoundWave> Sonido(TEXT("SoundWave'/Game/Free_Sounds_Pack/wav/Whoosh_4-1.Whoosh_4-1'"));
	if (Sonido.Succeeded())
		SonidoDestruccion = Sonido.Object;

	VelocidadMovimiento = 600.f;
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
	case 1: Path = TEXT("StaticMesh'/Game/RockEnv_Pack/Meshes/Cave_Rocks/SM_Rock_Cave_5.SM_Rock_Cave_5'"); break;
	case 2: Path = TEXT("StaticMesh'/Game/RockEnv_Pack/Meshes/Cave_Rocks/SM_Rock_Cave_4.SM_Rock_Cave_4'"); break;
	case 3: Path = TEXT("StaticMesh'/Game/RockEnv_Pack/Meshes/Cave_Rocks/SM_Rock_Cave_2.SM_Rock_Cave_2'"); break;
	case 4: Path = TEXT("StaticMesh'/Game/RockEnv_Pack/Meshes/Cave_Rocks/SM_Rock_Cave_1.SM_Rock_Cave_1'"); break;
	default: return;
	}

	UStaticMesh* Malla = LoadObject<UStaticMesh>(nullptr, *Path);
	if (Malla)
	{
		MeshComp->SetStaticMesh(Malla);
		float Escala = FMath::RandRange(0.5f, 1.5f);
		MeshComp->SetRelativeScale3D(FVector(Escala));
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
	NuevaPos.X -= VelocidadMovimiento * DeltaTime;
	SetActorLocation(NuevaPos);
}

void AMegaObstaculo::VerificarDestruccion()
{
	if (GetActorLocation().X <= -1100.000244f)
		Destroy();
}

void AMegaObstaculo::SetVelocidadMovimiento(float NuevaVelocidad)
{
	VelocidadMovimiento = NuevaVelocidad;
}