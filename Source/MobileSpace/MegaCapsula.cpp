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
	case 1: Path = TEXT("ParticleSystem'/Game/VE_PowerUps/Particles/PS_04_B_GoldBar.PS_04_B_GoldBar'"); break;
	case 2: Path = TEXT("ParticleSystem'/Game/VE_PowerUps/Particles/PS_04_Y_Lighting.PS_04_Y_Lighting'"); break;
	case 3: Path = TEXT("ParticleSystem'/Game/VE_PowerUps/Particles/PS_05_R_Knife.PS_05_R_Knife'"); break;
	case 4: Path = TEXT("ParticleSystem'/Game/VE_PowerUps/Particles/PS_GoldCoin01.PS_GoldCoin01'"); break;
	case 5: Path = TEXT("ParticleSystem'/Game/VE_PowerUps/Particles/PS_24_Y.PS_24_Y'"); break;
	case 6: Path = TEXT("ParticleSystem'/Game/VE_PowerUps/Particles/PS_25_B.PS_25_B'"); break;
	case 7: Path = TEXT("ParticleSystem'/Game/VE_PowerUps/Particles/PS_24_G.PS_24_G'"); break;
	case 8: Path = TEXT("ParticleSystem'/Game/VE_PowerUps/Particles/PS_27_G.PS_27_G'"); break;
	default: return;
	}

	UParticleSystem* Particula = LoadObject<UParticleSystem>(nullptr, *Path);
	if (Particula)
		ParticleComp->SetTemplate(Particula);
		ParticleComp->SetRelativeScale3D(FVector(3.f, 3.f, 3.f));

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