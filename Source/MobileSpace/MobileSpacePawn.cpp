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
#include "ProjectileZero.h"


const FName AMobileSpacePawn::MoveForwardBinding("MoveForward");
const FName AMobileSpacePawn::MoveRightBinding("MoveRight");
const FName AMobileSpacePawn::FireForwardBinding("FireForward");
const FName AMobileSpacePawn::FireRightBinding("FireRight");


AMobileSpacePawn::AMobileSpacePawn()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;
	CollisionComponent->InitCapsuleSize(250.f, 160.f);
	CollisionComponent->SetCollisionProfileName(TEXT("Pawn"));
	CollisionComponent->SetNotifyRigidBodyCollision(true);
	CollisionComponent->SetGenerateOverlapEvents(true);
	CollisionComponent->SetHiddenInGame(true);
	CollisionComponent->SetCollisionObjectType(ECC_Pawn);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Block);
	CollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);

	CapsuleBaseRadius = 250.f;
	CapsuleBaseHalfHeight = 160.f;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMesh(TEXT("StaticMesh'/Game/StarSparrow/Meshes/Examples/SM_StarSparrow09.SM_StarSparrow09'"));
	ShipMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	ShipMeshComponent->SetupAttachment(CollisionComponent);
	if (ShipMesh.Succeeded())
		ShipMeshComponent->SetStaticMesh(ShipMesh.Object);
	ShipMeshComponent->SetRelativeScale3D(FVector(0.45f));
	ShipMeshComponent->SetRelativeLocation(FVector(0.f, 0.f, -60.f));
	ShipMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ParticleTrail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleTrail"));
	ParticleTrail->SetupAttachment(ShipMeshComponent);
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("ParticleSystem'/Game/MagicProjectilesVol2/Particles/Projectiles/P_Projectile_ElectricBall01_Yellow.P_Projectile_ElectricBall01_Yellow'"));
	if (ParticleAsset.Succeeded())
	{
		ParticleTrail->SetTemplate(ParticleAsset.Object);
		ParticleTrail->SetRelativeLocation(FVector(-250.f, 0.f, 0.f));
		ParticleTrail->SetRelativeScale3D(FVector(1.0f));
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> EscudoFX(TEXT("/Game/GrimzaFX/Particles/NS_BubbleGex.NS_BubbleGex"));
	if (EscudoFX.Succeeded())
	{
		EscudoNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("EscudoNiagara"));
		EscudoNiagara->SetupAttachment(CollisionComponent);
		EscudoNiagara->SetAsset(EscudoFX.Object);
		EscudoNiagara->SetAutoActivate(false);
		EscudoNiagara->SetWorldScale3D(FVector(1.2f));
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> EscudoSnd(TEXT("SoundWave'/Game/Musica_D/EpicToonSFX/MagicCombatAudio/Magic/MagicField2_Normalloops.MagicField2_Normalloops'"));
	if (EscudoSnd.Succeeded())
		EscudoSound = EscudoSnd.Object;

	static ConstructorHelpers::FObjectFinder<USoundWave> VelSnd(TEXT("/Game/GoodFXLevelUp/SFX/Sound_Wave/A_GFXLU_lightning.A_GFXLU_lightning"));
	if (VelSnd.Succeeded())
		VelocidadSound = VelSnd.Object;

	static ConstructorHelpers::FObjectFinder<USoundWave> SndSobrecargaTotal(TEXT("SoundWave'/Game/GoodFXLevelUp/SFX/Sound_Wave/A_GFXLU_lightning.A_GFXLU_lightning'"));
	if (SndSobrecargaTotal.Succeeded())
		SoundSobrecargaTotal = SndSobrecargaTotal.Object;


	ExplosionFX = LoadObject<UParticleSystem>(nullptr, TEXT("/Game/FXVarietyPack/Particles/P_ky_waterBallHit.P_ky_waterBallHit"));
	ExplosionSound = LoadObject<USoundWave>(nullptr, TEXT("/Game/Musica_D/EpicToonSFX/MagicCombatAudio/NovaExplosion/Nova_Frost.Nova_Frost"));

	ProjectileClass = AProjectile_1::StaticClass();
	MoveSpeed = 1800.f;
	VelocidadNormal = MoveSpeed;
	GunOffset = FVector(230.f, 0.f, 0.f);
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

	if (OtherActor->IsA(AProjectileZero::StaticClass()))
	{
		Cast<AProjectileZero>(OtherActor)->DestroyProjectile();

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
	bPresionandoDisparo = (FMath::Abs(FireForwardValue) > 0.0f || FMath::Abs(FireRightValue) > 0.0f);

	if (bPresionandoDisparo)
		FireShot(FVector(1.f, 0.f, 0.f));

	
	FVector ActorLocation = GetActorLocation();
	ActorLocation.X = FMath::Clamp(ActorLocation.X, MovementMin.X, MovementMax.X);
	ActorLocation.Y = FMath::Clamp(ActorLocation.Y, MovementMin.Y, MovementMax.Y);
	SetActorLocation(ActorLocation);


	if (!bPresionandoDisparo && SobrecargaActual > 0.f)
	{
		SobrecargaActual -= TasaEnfriamiento * DeltaSeconds;
		SobrecargaActual = FMath::Max(SobrecargaActual, 0.f);

		if (bEstaSobrecargado && SobrecargaActual <= 0.f)
		{
			bEstaSobrecargado = false;
			bCanFire = true;
		}
	}


	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{
		AHUDmain* HUD = Cast<AHUDmain>(PC->GetHUD());
		if (HUD && HUD->WidgetOnGameInstance)
			HUD->WidgetOnGameInstance->ActualizarSobrecarga(SobrecargaActual, SobrecargaMax);
	}

	if (CantVida <= 0)
	{
		Destroy();
		APlayerController* PC2 = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (!PC2) return;

		if (AHUDmain* HUD = Cast<AHUDmain>(PC2->GetHUD()))
			HUD->MostrarGameOver();
	}
}


void AMobileSpacePawn::FireShot(FVector FireDirection)
{
	if (bInmuneEscudo || bEstaSobrecargado || !bCanFire || !ProjectileClass) return;

	UWorld* World = GetWorld();
	if (!World) return;


	SobrecargaActual += TasaSobrecarga;
	SobrecargaActual = FMath::Clamp(SobrecargaActual, 0.f, SobrecargaMax);


	if (SobrecargaActual >= SobrecargaMax)
	{
		bEstaSobrecargado = true;
		bCanFire = false;

		if (SoundSobrecargaTotal)
			UGameplayStatics::PlaySoundAtLocation(this, SoundSobrecargaTotal, GetActorLocation());

		return;
	}

	const FRotator BaseRotation = FireDirection.Rotation();
	FVector BaseLocation = GetActorLocation() + BaseRotation.RotateVector(GunOffset);

	int32 NumDisparos = FMath::Clamp(NivelEvolucion, 1, 5);
	TArray<float> Angulos;
	switch (NumDisparos)
	{
	case 1: Angulos = { 0.f }; break;
	case 2: Angulos = { -10.f, 10.f }; break;
	case 3: Angulos = { -17.f, 0.f, 17.f }; break;
	case 4: Angulos = { -22.f, -10.f, 10.f, 22.f }; break;
	case 5: Angulos = { -20.f, -10.f, 0.f, 10.f, 20.f }; break;
	default: Angulos = { 0.f }; break;
	}

	for (float AngleOffset : Angulos)
	{
		FRotator ShotRotation = BaseRotation + FRotator(0.f, AngleOffset, 0.f);
		FVector SpawnLocation = BaseLocation + ShotRotation.Vector() * 100.f;

		AMobileSpaceProjectile* Proj = World->SpawnActor<AMobileSpaceProjectile>(
			ProjectileClass,
			SpawnLocation,
			ShotRotation
		);

		if (Proj)
		{
			Proj->SetWeaponAndEvolution(TipoArmaActual, NivelEvolucion);
			Proj->PlayFireSound();
		}
	}

	bCanFire = false;
	World->GetTimerManager().SetTimer(
		TimerHandle_ShotTimerExpired,
		this,
		&AMobileSpacePawn::ShotTimerExpired,
		FireRate
	);
}



void AMobileSpacePawn::ShotTimerExpired()
{
	bCanFire = true;
}

void AMobileSpacePawn::DispararMisil()
{
	UWorld* World = GetWorld();
	if (!World) return;

	FVector ForwardDir = ShipMeshComponent->GetForwardVector();
	FVector SpawnLocation = GetActorLocation() + ForwardDir * 200.f + FVector(0.f, 0.f, 50.f);
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
	case 1: // VIDA
		CantVida += 1;
		Widget->ActualizarVida(CantVida);
		break;

	case 2: // VELOCIDAD
		CantVelocidad += 1;
		Widget->ActualizarVelocidad(CantVelocidad);
		break;

	case 3: // MISIL
		CantMissil += 1;
		Widget->ActualizarMisiles(CantMissil);
		break;

	case 4: // ESCUDO
		CantEscudo += 1;
		Widget->ActualizarEscudo(CantEscudo);
		break;

	case 5: // Arma1
		if (TipoArmaActual == EWeaponType::Arma1)
			NivelEvolucion = FMath::Clamp(NivelEvolucion + 1, 1, 3);
		else
		{
			TipoArmaActual = EWeaponType::Arma1;
			NivelEvolucion = 1;
		}
		Widget->ActualizarArmaActual(1);
		break;

	case 6: // Arma2
		if (TipoArmaActual == EWeaponType::Arma2)
			NivelEvolucion = FMath::Clamp(NivelEvolucion + 1, 1, 3);
		else
		{
			TipoArmaActual = EWeaponType::Arma2;
			NivelEvolucion = 1;
		}
		Widget->ActualizarArmaActual(2);
		break;

	case 7: // Arma3
		if (TipoArmaActual == EWeaponType::Arma3)
			NivelEvolucion = FMath::Clamp(NivelEvolucion + 1, 1, 3);
		else
		{
			TipoArmaActual = EWeaponType::Arma3;
			NivelEvolucion = 1;
		}
		Widget->ActualizarArmaActual(3);
		break;

	case 8: // Arma4
		if (TipoArmaActual == EWeaponType::Arma4)
			NivelEvolucion = FMath::Clamp(NivelEvolucion + 1, 1, 3);
		else
		{
			TipoArmaActual = EWeaponType::Arma4;
			NivelEvolucion = 1;
		}
		Widget->ActualizarArmaActual(4);
		break;

	default:
		break;
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
	bCanFire = false;

	if (EscudoNiagara)
	{
		EscudoNiagara->SetVisibility(true);
		EscudoNiagara->Activate(true);
		EscudoNiagara->SetWorldScale3D(FVector(0.8f));
	}

	if (EscudoSound)
		UGameplayStatics::SpawnSoundAttached(EscudoSound, RootComponent);

	CollisionComponent->SetCapsuleSize(CapsuleBaseRadius * 1.5f, CapsuleBaseHalfHeight * 1.5f, true);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Block);
	CollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);

	GetWorldTimerManager().SetTimer(TimerHandle_Escudo, this, &AMobileSpacePawn::DesactivarEscudo, 8.0f, false);
}

void AMobileSpacePawn::DesactivarEscudo()
{
	bInmuneEscudo = false;
	bCanFire = true;

	CollisionComponent->SetCapsuleSize(CapsuleBaseRadius, CapsuleBaseHalfHeight, true);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Block);
	CollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);

	if (EscudoNiagara)
	{
		EscudoNiagara->Deactivate();
		EscudoNiagara->SetVisibility(false);
		EscudoNiagara->SetWorldScale3D(FVector(1.2f));
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
