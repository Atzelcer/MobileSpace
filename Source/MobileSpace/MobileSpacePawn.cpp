// Copyright Epic Games, Inc. All Rights Reserved.

#include "MobileSpacePawn.h"
#include "MobileSpaceProjectile.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Particles/ParticleSystemComponent.h"

const FName AMobileSpacePawn::MoveForwardBinding("MoveForward");
const FName AMobileSpacePawn::MoveRightBinding("MoveRight");
const FName AMobileSpacePawn::FireForwardBinding("FireForward");
const FName AMobileSpacePawn::FireRightBinding("FireRight");

AMobileSpacePawn::AMobileSpacePawn()
{	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMesh(TEXT("StaticMesh'/Game/StarSparrow/Meshes/Examples/SM_StarSparrow19.SM_StarSparrow19'"));
	// Create the mesh component
	ShipMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	RootComponent = ShipMeshComponent;
	ShipMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	ShipMeshComponent->SetStaticMesh(ShipMesh.Object);
	ShipMeshComponent->SetRelativeScale3D(FVector(0.3f, 0.3f, 0));
	
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
	

}

void AMobileSpacePawn::FireShot(FVector FireDirection)
{
	// If it's ok to fire again
	if (bCanFire == true)
	{
		// Always fire forward in Galaga style
		const FRotator FireRotation = FVector(1.f, 0.f, 0.f).Rotation(); // Always forward
		
		// Spawn projectile at an offset from this pawn
		const FVector SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);

		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			// spawn the projectile
			World->SpawnActor<AMobileSpaceProjectile>(SpawnLocation, FireRotation);
		}

		bCanFire = false;
		World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &AMobileSpacePawn::ShotTimerExpired, FireRate);

		// try and play the sound if specified
		if (FireSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		}
	}
}

void AMobileSpacePawn::ShotTimerExpired()
{
	bCanFire = true;
}

