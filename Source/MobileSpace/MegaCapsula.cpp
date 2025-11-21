// Fill out your copyright notice in the Description page of Project Settings.


#include "MegaCapsula.h"
#include "Kismet/GameplayStatics.h"
#include "MobileSpacePawn.h"
#include "MegaObstaculo.h"

AMegaCapsula::AMegaCapsula()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComp"));
	RootComponent = CollisionComp;
	CollisionComp->InitCapsuleSize(50.f, 100.f);
	CollisionComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComp"));
	ParticleComp->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<USoundWave> Sonido(TEXT("SoundWave'/Game/Free_Sounds_Pack/wav/Whoosh_4-1.Whoosh_4-1'"));
	if (Sonido.Succeeded())
		SonidoRecoleccion = Sonido.Object;

	Velocidad = 320.f;
}

void AMegaCapsula::BeginPlay()
{
	Super::BeginPlay();
	AsignarParticulaAleatoria();
}

void AMegaCapsula::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoverCapsula(DeltaTime);
	VerificarDestruccion();
}

void AMegaCapsula::AsignarParticulaAleatoria()
{
	TipoCapsula = FMath::RandRange(1, 8);
	FString Path;

	switch (TipoCapsula)
	{
	case 1: Path = TEXT("ParticleSystem'/Game/VE_PowerUps/Particles/P_Omega08.P_Omega08'"); break;
	case 2: Path = TEXT("ParticleSystem'/Game/VE_PowerUps/Particles/P_Omega05.P_Omega05'"); break;
	case 3: Path = TEXT("ParticleSystem'/Game/VE_PowerUps/Particles/P_Omega06.P_Omega06'"); break;
	case 4: Path = TEXT("ParticleSystem'/Game/VE_PowerUps/Particles/P_Omega07.P_Omega07'"); break;
	case 5: Path = TEXT("ParticleSystem'/Game/VE_PowerUps/Particles/P_Omega02.P_Omega02'"); break;
	case 6: Path = TEXT("ParticleSystem'/Game/VE_PowerUps/Particles/P_Omega03.P_Omega03'"); break;
	case 7: Path = TEXT("ParticleSystem'/Game/VE_PowerUps/Particles/P_Omega04.P_Omega04'"); break;
	case 8: Path = TEXT("ParticleSystem'/Game/VE_PowerUps/Particles/P_Omega01.P_Omega01'"); break;
	default: return;
	}

	UParticleSystem* Particula = LoadObject<UParticleSystem>(nullptr, *Path);
	if (Particula)
		ParticleComp->SetTemplate(Particula);
		ParticleComp->SetRelativeScale3D(FVector(4.f, 2.f, 2.f));

}

void AMegaCapsula::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	AMobileSpacePawn* Nave = Cast<AMobileSpacePawn>(OtherActor);
	if (Nave)
	{
		Nave->EstablecerCapsula(TipoCapsula);

		DestruirCapsula();
	}
}

void AMegaCapsula::DestruirCapsula()
{
	if (SonidoRecoleccion)
		UGameplayStatics::PlaySoundAtLocation(this, SonidoRecoleccion, GetActorLocation());

	Destroy();
}


void AMegaCapsula::MoverCapsula(float DeltaTime)
{
	FVector NuevaPos = GetActorLocation();
	NuevaPos.X -= Velocidad * DeltaTime;
	SetActorLocation(NuevaPos);
}

void AMegaCapsula::VerificarDestruccion()
{
	if (GetActorLocation().X <= -1400.000244f)
		Destroy();
}