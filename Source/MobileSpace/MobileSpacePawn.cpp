#include "MobileSpacePawn.h"
#include "MobileSpaceProjectile.h"
#include "Projectile_1.h"
#include "MegaMIssil.h"
#include "HUDmain.h"
#include "Widget_ON_GAME.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/CollisionProfile.h"
#include "Sound/SoundWave.h"
#include "TimerManager.h"
#include "Components/BoxComponent.h"
#include "AventuraManager.h"
#include "Ship_X.h"
#include "MegaObstaculo.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "MegaCapsula.h"


const FName AMobileSpacePawn::MoveForwardBinding("MoveForward");
const FName AMobileSpacePawn::MoveRightBinding("MoveRight");
const FName AMobileSpacePawn::FireForwardBinding("FireForward");
const FName AMobileSpacePawn::FireRightBinding("FireRight");


AMobileSpacePawn::AMobileSpacePawn()
{
	// --- Colisión principal ---
	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;
	CollisionComponent->InitCapsuleSize(250.f, 160.f); // ajustado al tamaño visual del mesh
	CollisionComponent->SetCollisionProfileName(TEXT("Pawn"));
	CollisionComponent->SetNotifyRigidBodyCollision(true);
	CollisionComponent->SetGenerateOverlapEvents(true);
	CollisionComponent->SetHiddenInGame(true);

	// --- Malla de la nave ---
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMesh(TEXT("StaticMesh'/Game/StarSparrow/Meshes/Examples/SM_StarSparrow09.SM_StarSparrow09'"));
	ShipMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	ShipMeshComponent->SetupAttachment(CollisionComponent);
	if (ShipMesh.Succeeded())
	{
		ShipMeshComponent->SetStaticMesh(ShipMesh.Object);
	}

	// Escala reducida del mesh (pero alineada con la colisión)
	ShipMeshComponent->SetRelativeScale3D(FVector(0.45f));
	ShipMeshComponent->SetRelativeLocation(FVector(0.f, 0.f, -60.f)); // centrado dentro de la cápsula
	ShipMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// --- Partícula propulsora ---
	ParticleTrail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleTrail"));
	ParticleTrail->SetupAttachment(ShipMeshComponent);
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("ParticleSystem'/Game/MagicProjectilesVol2/Particles/Projectiles/P_Projectile_ElectricBall01_Yellow.P_Projectile_ElectricBall01_Yellow'"));
	if (ParticleAsset.Succeeded())
	{
		ParticleTrail->SetTemplate(ParticleAsset.Object);
		ParticleTrail->SetRelativeLocation(FVector(-250.f, 0.f, 0.f));
		ParticleTrail->SetRelativeScale3D(FVector(1.0f));
	}

	// --- FX Escudo ---
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> EscudoFX(TEXT("/Game/GrimzaFX/Particles/NS_BubbleGex.NS_BubbleGex"));
	if (EscudoFX.Succeeded())
	{
		EscudoNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("EscudoNiagara"));
		EscudoNiagara->SetupAttachment(CollisionComponent);
		EscudoNiagara->SetAsset(EscudoFX.Object);
		EscudoNiagara->SetAutoActivate(false);
		EscudoNiagara->SetWorldScale3D(FVector(1.2f));
	}

	// --- Sonidos ---
	static ConstructorHelpers::FObjectFinder<USoundWave> EscudoSnd(TEXT("SoundWave'/Game/Musica_D/EpicToonSFX/MagicCombatAudio/Magic/MagicField2_Normalloops.MagicField2_Normalloops'"));
	if (EscudoSnd.Succeeded())
		EscudoSound = EscudoSnd.Object;

	static ConstructorHelpers::FObjectFinder<USoundWave> VelSnd(TEXT("/Game/GoodFXLevelUp/SFX/Sound_Wave/A_GFXLU_lightning.A_GFXLU_lightning"));
	if (VelSnd.Succeeded())
		VelocidadSound = VelSnd.Object;

	ExplosionFX = LoadObject<UParticleSystem>(nullptr, TEXT("/Game/FXVarietyPack/Particles/P_ky_waterBallHit.P_ky_waterBallHit"));
	ExplosionSound = LoadObject<USoundWave>(nullptr, TEXT("/Game/Musica_D/EpicToonSFX/MagicCombatAudio/NovaExplosion/Nova_Frost.Nova_Frost"));

	// --- Propiedades de juego ---
	ProjectileClass = AProjectile_1::StaticClass();
	MoveSpeed = 1800.f;
	VelocidadNormal = MoveSpeed;
	GunOffset = FVector(380.f, 0.f, 0.f);
	FireRate = 0.2f;
	bCanFire = true;
	bInmuneEscudo = false;
	bHacerDanio = true;

	CantEscudo = 3;
	CantMissil = 3;
	CantVelocidad = 3;
	CantVida = 3;
}



void AMobileSpacePawn::BeginPlay()
{
	Super::BeginPlay();
}

void AMobileSpacePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis(MoveForwardBinding);
	PlayerInputComponent->BindAxis(MoveRightBinding);
	PlayerInputComponent->BindAxis(FireForwardBinding);
	PlayerInputComponent->BindAxis(FireRightBinding);
}

void AMobileSpacePawn::NotifyActorBeginOverlap(AActor* OtherActor)
{

	if (OtherActor->IsA(AShip_X::StaticClass()))
	{
		Cast<AShip_X>(OtherActor)->HandleDestruction();

		HacerDanio();
	}


	if (OtherActor->IsA(AMegaObstaculo::StaticClass()))
	{
		Cast<AMegaObstaculo>(OtherActor)->DestruirObstaculo();

		HacerDanio();
	}
}

void AMobileSpacePawn::ActivarDanio()
{
	bHacerDanio = true;
}


void AMobileSpacePawn::Tick(float DeltaSeconds)
{
	const float ForwardValue = GetInputAxisValue(MoveForwardBinding);
	const float RightValue = GetInputAxisValue(MoveRightBinding);
	const FVector Movement = FVector(ForwardValue, RightValue, 0.f) * MoveSpeed * DeltaSeconds;

	const FRotator CurrentRotation = GetActorRotation();
	FRotator SmoothRotation = FMath::RInterpTo(CurrentRotation, FRotator(0, 0, -RightValue * 15.f), DeltaSeconds, 5.0f);
	SetActorRotation(SmoothRotation);

	if (Movement.SizeSquared() > 0.0f)
	{
		FHitResult Hit(1.f);
		RootComponent->MoveComponent(Movement, SmoothRotation, true, &Hit);
		if (Hit.IsValidBlockingHit())
		{
			const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
			const FVector Deflection = FVector::VectorPlaneProject(Movement, Normal2D) * (1.f - Hit.Time);
			RootComponent->MoveComponent(Deflection, SmoothRotation, true);
		}
	}

	const float FireForwardValue = GetInputAxisValue(FireForwardBinding);
	const float FireRightValue = GetInputAxisValue(FireRightBinding);
	if (FMath::Abs(FireForwardValue) > 0.0f || FMath::Abs(FireRightValue) > 0.0f)
		FireShot(FVector(1.f, 0.f, 0.f));

	FVector ActorLocation = GetActorLocation();
	ActorLocation.X = FMath::Clamp(ActorLocation.X, MovementMin.X, MovementMax.X);
	ActorLocation.Y = FMath::Clamp(ActorLocation.Y, MovementMin.Y, MovementMax.Y);
	SetActorLocation(ActorLocation);

	if (CantVida <= 0)
	{
		Destroy();
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (!PC) return;

		if (AHUDmain* HUD = Cast<AHUDmain>(PC->GetHUD()))
			HUD->MostrarGameOver();

	}
}

void AMobileSpacePawn::FireShot(FVector FireDirection)
{
	if (!bCanFire || !ProjectileClass) return;
	UWorld* World = GetWorld();
	if (!World) return;

	const FRotator FireRotation = FireDirection.Rotation();
	FVector SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);

	AMobileSpaceProjectile* Proj = World->SpawnActor<AMobileSpaceProjectile>(ProjectileClass, SpawnLocation, FireRotation);
	if (Proj)
		Proj->PlayFireSound();

	bCanFire = false;
	World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &AMobileSpacePawn::ShotTimerExpired, FireRate);
}

void AMobileSpacePawn::ShotTimerExpired()
{
	bCanFire = true;
}

void AMobileSpacePawn::DispararMisil()
{
	UWorld* World = GetWorld();
	if (!World) return;

	FVector SpawnLocation = GetActorLocation() + FVector(85.471046f, 0.0f, 300.0f);
	FRotator SpawnRotation = (FVector(85.471046f, 0.0f, 300.0f) - SpawnLocation).Rotation();

	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.Instigator = GetInstigator();

	World->SpawnActor<AMegaMIssil>(AMegaMIssil::StaticClass(), SpawnLocation, SpawnRotation, Params);
}

void AMobileSpacePawn::InicializarPowerUpsHUD()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC) return;

	AHUDmain* HUD = Cast<AHUDmain>(PC->GetHUD());
	if (!HUD) return;

	UWidget_ON_GAME* Widget = HUD->WidgetOnGameInstance;
	if (!Widget || !Widget->IsInViewport()) return;

	Widget->ActualizarVida(CantVida);
	Widget->ActualizarVelocidad(CantVelocidad);
	Widget->ActualizarMisiles(CantMissil);
	Widget->ActualizarEscudo(CantEscudo);
}

void AMobileSpacePawn::EstablecerCapsula(int32 TipoCapsula)
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC) return;

	AHUDmain* HUD = Cast<AHUDmain>(PC->GetHUD());
	if (!HUD) return;

	UWidget_ON_GAME* Widget = HUD->WidgetOnGameInstance;
	if (!Widget || !Widget->IsInViewport()) return;

	switch (TipoCapsula)
	{
	case 1: CantVida += 1; Widget->ActualizarVida(CantVida); break;
	case 2: CantVelocidad += 1; Widget->ActualizarVelocidad(CantVelocidad); break;
	case 3: CantMissil += 1; Widget->ActualizarMisiles(CantMissil); break;
	case 4: CantEscudo += 1; Widget->ActualizarEscudo(CantEscudo); break;
	default: break;
	}
}

void AMobileSpacePawn::HacerDanio()
{
	if (bInmuneEscudo) return;

	if(bHacerDanio == true)
	{

		if (ExplosionFX)
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionFX, GetActorLocation(), FRotator::ZeroRotator, FVector(2.f));

		if (ExplosionSound)
			UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());

		for (TActorIterator<AAventuraManager> It(GetWorld()); It; ++It)
		{
			if (AAventuraManager* Manager = *It)
			{
				Manager->TeletransportarJugador();
			}
		}

		CantVida -= 1;
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (!PC) return;

		AHUDmain* HUD = Cast<AHUDmain>(PC->GetHUD());
		if (!HUD) return;

		UWidget_ON_GAME* Widget = HUD->WidgetOnGameInstance;
		if (!Widget || !Widget->IsInViewport()) return;
		if (Widget)
			Widget->ActualizarVida(CantVida);
		bHacerDanio = false;
		GetWorldTimerManager().SetTimer(TimerHandle_hacerDanio, this, &AMobileSpacePawn::ActivarDanio, 5.f, false);
	}
}

void AMobileSpacePawn::ActivarEscudo()
{
	if (bInmuneEscudo)
		return;

	bInmuneEscudo = true;

	if (EscudoNiagara)
	{
		EscudoNiagara->SetVisibility(true);
		EscudoNiagara->Activate(true);
		EscudoNiagara->SetWorldScale3D(FVector(0.8f));
	}

	if (EscudoSound)
		UGameplayStatics::SpawnSoundAttached(EscudoSound, RootComponent);

	CollisionComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);

	float BaseRadius, BaseHalfHeight;
	CollisionComponent->GetUnscaledCapsuleSize(BaseRadius, BaseHalfHeight);
	CollisionComponent->SetCapsuleSize(BaseRadius * 1.5f, BaseHalfHeight * 1.5f);

	GetWorldTimerManager().SetTimer(TimerHandle_Escudo, this, &AMobileSpacePawn::DesactivarEscudo, 8.0f, false);
}

void AMobileSpacePawn::DesactivarEscudo()
{
	bInmuneEscudo = false;

	CollisionComponent->InitCapsuleSize(250.f, 160.f);
	CollisionComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);

	if (EscudoNiagara)
	{
		EscudoNiagara->Deactivate();
		EscudoNiagara->SetVisibility(false);
		EscudoNiagara->SetWorldScale3D(FVector(0.45f));
	}

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC) return;

	AHUDmain* HUD = Cast<AHUDmain>(PC->GetHUD());
	if (!HUD) return;

	UWidget_ON_GAME* Widget = HUD->WidgetOnGameInstance;
	if (!Widget || !Widget->IsInViewport()) return;

	Widget->ReactivarEscudo();
}


void AMobileSpacePawn::ActivarVelocidad()
{
	if (VelocidadSound)
		UGameplayStatics::PlaySoundAtLocation(this, VelocidadSound, GetActorLocation());

	MoveSpeed = VelocidadNormal * 2.0f;
	GetWorldTimerManager().SetTimer(TimerHandle_Velocidad, this, &AMobileSpacePawn::DesactivarVelocidad, 10.0f, false);
}



void AMobileSpacePawn::DesactivarVelocidad()
{
	MoveSpeed = VelocidadNormal;

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC) return;

	AHUDmain* HUD = Cast<AHUDmain>(PC->GetHUD());
	if (!HUD) return;

	UWidget_ON_GAME* Widget = HUD->WidgetOnGameInstance;
	if (!Widget || !Widget->IsInViewport()) return;

	Widget->ReactivarVelocidad();
}
