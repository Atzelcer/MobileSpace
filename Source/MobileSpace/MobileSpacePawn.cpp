// Copyright Epic Games, Inc. All Rights Reserved.

#include "MobileSpacePawn.h"
#include "MobileSpaceProjectile.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Particles/ParticleSystemComponent.h"
#include "HUDmain.h"
#include "EngineUtils.h"
#include "Widget_ON_GAME.h"


const FName AMobileSpacePawn::MoveForwardBinding("MoveForward");
const FName AMobileSpacePawn::MoveRightBinding("MoveRight");
const FName AMobileSpacePawn::FireForwardBinding("FireForward");
const FName AMobileSpacePawn::FireRightBinding("FireRight");

AMobileSpacePawn::AMobileSpacePawn()
{	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMesh(TEXT("StaticMesh'/Game/StarSparrow/Meshes/Examples/SM_StarSparrow09.SM_StarSparrow09'"));
	// Create the mesh component
	ShipMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	RootComponent = ShipMeshComponent;
	ShipMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	ShipMeshComponent->SetStaticMesh(ShipMesh.Object);
	ShipMeshComponent->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));

	// Cache our sound effect
	static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("SoundWave'/Game/Free_Sounds_Pack/wav/Sci-Fi_Gun_1-1.Sci-Fi_Gun_1-1'"));
	FireSound = FireAudio.Object;

	// No camera setup - using fixed camera from GameMode

	// Movement
	MoveSpeed = 1800.0f;
	// Weapon
	GunOffset = FVector(90.f, 0.f, 0.f);
	FireRate = 0.1f;
	bCanFire = true;

	//partycle
	ParticleTrail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleTrail"));
	ParticleTrail->SetupAttachment(ShipMeshComponent);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("ParticleSystem'/Game/MagicProjectilesVol2/Particles/Projectiles/P_Projectile_ElectricBall01_Yellow.P_Projectile_ElectricBall01_Yellow'"));
	if (ParticleAsset.Succeeded())
	{
		ParticleTrail->SetTemplate(ParticleAsset.Object);
		ParticleTrail->SetRelativeLocation(FVector(-500.f, 0.f, 0.f));
		ParticleTrail->SetRelativeScale3D(FVector(2.f, 2.f, 2.f));
		
	}

	ProjectileClass = AMobileSpaceProjectile::StaticClass();
}

void AMobileSpacePawn::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (!World) return;

	for (TActorIterator<AHUDmain> It(World); It; ++It)
	{
		HUDRef = *It;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("HUD ya existe "));
		break;
	}

	if (!HUDRef)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		if (PC)
			HUDRef = Cast<AHUDmain>(PC->GetHUD());
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Asignando HUD desde PlayerController "));
	}
	EstablecerCapsula(2);
}


void AMobileSpacePawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	// set up gameplay key bindings
	PlayerInputComponent->BindAxis(MoveForwardBinding);
	PlayerInputComponent->BindAxis(MoveRightBinding);
	PlayerInputComponent->BindAxis(FireForwardBinding);
	PlayerInputComponent->BindAxis(FireRightBinding);
}

void AMobileSpacePawn::Tick(float DeltaSeconds)
{
	// Galaga-style controls: Movement in both axes but no rotation
	const float ForwardValue = GetInputAxisValue(MoveForwardBinding);  // W/S movement
	const float RightValue = GetInputAxisValue(MoveRightBinding);      // A/D movement

	// Create movement in both X (forward/back) and Y (left/right) axes
	const FVector Movement = FVector(ForwardValue, RightValue, 0.f) * MoveSpeed * DeltaSeconds;

	// Get current rotation once to avoid duplicate declarations
	const FRotator CurrentRotation = GetActorRotation();
	FRotator SmoothRotation;

	// Banking effect: Soft tilt the ship based on horizontal movement
	if (FMath::Abs(RightValue) > 0.0f)
	{
		// Apply banking when moving horizontally
		const float BankingAngle = RightValue * 15.0f; // Maximum 15 degrees of banking (more subtle)
		const FRotator TargetRotation = FRotator(0.f, 0.f, -BankingAngle); // Roll rotation for banking
		SmoothRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaSeconds, 5.0f);
	}
	else
	{
		// If not moving horizontally, gradually return to neutral position
		const FRotator NeutralRotation = FRotator(0.f, 0.f, 0.f);
		SmoothRotation = FMath::RInterpTo(CurrentRotation, NeutralRotation, DeltaSeconds, 4.0f);
	}
	
	// Apply the smooth rotation
	SetActorRotation(SmoothRotation);

	// If non-zero size, move this actor
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
	
	// Galaga-style shooting: Check for fire input (any fire input shoots upward)
	const float FireForwardValue = GetInputAxisValue(FireForwardBinding);
	const float FireRightValue = GetInputAxisValue(FireRightBinding);
	
	// If any fire input is detected, shoot forward (upward in Galaga style)
	if (FMath::Abs(FireForwardValue) > 0.0f || FMath::Abs(FireRightValue) > 0.0f)
	{
		// Always shoot forward (X direction)
		const FVector FireDirection = FVector(1.f, 0.f, 0.f);
		FireShot(FireDirection);
	}
	FVector ActorLocation = GetActorLocation();
	ActorLocation.X = FMath::Clamp(ActorLocation.X, MovementMin.X, MovementMax.X);
	ActorLocation.Y = FMath::Clamp(ActorLocation.Y, MovementMin.Y, MovementMax.Y);
	SetActorLocation(ActorLocation);
	

}

void AMobileSpacePawn::FireShot(FVector FireDirection)
{
	if (!bCanFire || !ProjectileClass) return;

	UWorld* World = GetWorld();
	if (!World) return;

	// Direcciones de disparo:
	const FRotator FireRotationCenter = FireDirection.Rotation();

	// Ángulo en grados para los lados
	float SideAngle = 30.f;

	// Rotaciones para los dos lados
	FRotator FireRotationLeft = FireRotationCenter + FRotator(0.f, -SideAngle, 0.f);
	FRotator FireRotationRight = FireRotationCenter + FRotator(0.f, SideAngle, 0.f);

	// Offset para que salgan separadas
	FVector OffsetLeft(0.f, -50.f, 0.f);   // Y negativo (izquierda)
	FVector OffsetRight(0.f, 50.f, 0.f);   // Y positivo (derecha)

	FVector SpawnLocation = GetActorLocation() + FireRotationCenter.RotateVector(GunOffset);

	// --- Disparo Izquierda ---
	FVector SpawnLocationLeft = SpawnLocation + OffsetLeft;
	if (MuzzleParticleAsset)
		UGameplayStatics::SpawnEmitterAtLocation(World, MuzzleParticleAsset, SpawnLocationLeft, FireRotationLeft);

	World->SpawnActor<AMobileSpaceProjectile>(ProjectileClass, SpawnLocationLeft, FireRotationLeft);

	// --- Disparo Derecha ---
	FVector SpawnLocationRight = SpawnLocation + OffsetRight;
	if (MuzzleParticleAsset)
		UGameplayStatics::SpawnEmitterAtLocation(World, MuzzleParticleAsset, SpawnLocationRight, FireRotationRight);

	World->SpawnActor<AMobileSpaceProjectile>(ProjectileClass, SpawnLocationRight, FireRotationRight);

	bCanFire = false;
	World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &AMobileSpacePawn::ShotTimerExpired, FireRate);

	if (FireSound)
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	
}

void AMobileSpacePawn::ShotTimerExpired()
{
	bCanFire = true;
}


void AMobileSpacePawn::EstablecerCapsula(int32 TipoCapsula)
{
	if (!HUDRef || !HUDRef->WidgetOnGameInstance)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("No se pudo establecer la capsula: Referencia HUD o Widget nula"));
		return;
	}

	switch (TipoCapsula)
	{
	case 1:
		NumLifes += 1;
		HUDRef->WidgetOnGameInstance->ActualizarVida(NumLifes);
		break;

	case 2:
		MoveSpeed += 1;
		HUDRef->WidgetOnGameInstance->ActualizarVelocidad(MoveSpeed);
		break;

	case 3:
		HUDRef->WidgetOnGameInstance->ActualizarMisiles(10);
		break;

	case 4:
		HUDRef->WidgetOnGameInstance->ActualizarEscudo(1);
		break;

	default:
		break;
	}
}
