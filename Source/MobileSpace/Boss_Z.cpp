// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss_Z.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Animation/AnimInstance.h"
#include "TimerManager.h"
#include "MobileSpaceProjectile.h"


// Sets default values
ABoss_Z::ABoss_Z()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	BossMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BossMesh"));
	BossMesh->SetupAttachment(RootComponent);

	ShipCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ShipCollision"));
	ShipCollision->SetupAttachment(RootComponent);
	ShipCollision->SetBoxExtent(FVector(300.f, 300.f, 300.f));
	
	// Configure collision properly
	ShipCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ShipCollision->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	ShipCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ShipCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	ShipCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	
	// Initialize animation pointers
	IdleAnimation = nullptr;
	AttackAnimation = nullptr;
	DeathAnimation = nullptr;
	
	// Initialize state
	CurrentState = EBossState::Idle;
}

// Called when the game starts or when spawned
void ABoss_Z::BeginPlay()
{
	Super::BeginPlay();
	
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, TEXT("Boss BeginPlay called!"));
	}
	
	// Setup collision
	ShipCollision->OnComponentBeginOverlap.AddDynamic(this, &ABoss_Z::OnBossHit);
	
	// Double-check collision setup
	ShipCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ShipCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ShipCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	ShipCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Boss collision setup complete!"));
	}
	
	// Start the boss behavior sequence
	StartIdlePhase();
}

// Called every frame
void ABoss_Z::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABoss_Z::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABoss_Z::StartIdlePhase()
{
	CurrentState = EBossState::Idle;
	
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Starting IDLE Phase"));
	//}
	//
	if (IdleAnimation)
	{
		//if (GEngine)
		//{
		//	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("IdleAnimation found! Playing..."));
		//}
		//
		// Try different methods to play animation
		UAnimInstance* AnimInstance = BossMesh->GetAnimInstance();
		if (AnimInstance)
		{
			//if (GEngine)
			//{
			//	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("AnimInstance found! Trying PlayAnimation..."));
			//}
			
			// Method 1: PlayAnimation (for meshes without AnimBP)
			BossMesh->PlayAnimation(IdleAnimation, true);
		}
		else
		{
			//if (GEngine)
			//{
			//	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange, TEXT("No AnimInstance - trying SetAnimation..."));
			//}
			
			// Method 2: SetAnimation (alternative method)
			BossMesh->SetAnimation(IdleAnimation);
		}
	}
	else
	{
		//if (GEngine)
		//{
		//	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("No IdleAnimation found!"));
		//}
	}
	
	// Set timer to transition to attack phase after IDLE_DURATION seconds
	GetWorldTimerManager().SetTimer(IdleTimerHandle, this, &ABoss_Z::StartAttackPhase, IDLE_DURATION, false);
}

void ABoss_Z::StartAttackPhase()
{
	CurrentState = EBossState::Attack;
	
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Starting ATTACK Phase"));
	//}
	
	if (AttackAnimation)
	{
		//if (GEngine)
		//{
		//	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("AttackAnimation found! Playing..."));
		//}
		//
		UAnimInstance* AnimInstance = BossMesh->GetAnimInstance();
		if (AnimInstance)
		{
			BossMesh->PlayAnimation(AttackAnimation, true); // Loop attack animation
		}
		else
		{
			//if (GEngine)
			//{
			//	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange, TEXT("No AnimInstance for Attack - trying SetAnimation..."));
			//}
			BossMesh->SetAnimation(AttackAnimation);
		}
	}
	else
	{
		//if (GEngine)
		//{
		//	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("No AttackAnimation found!"));
		//}
	}
}

void ABoss_Z::StartDeathPhase()
{
	CurrentState = EBossState::Death;
	
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Magenta, TEXT("Starting DEATH Phase"));
	//}
	
	// Clear any existing timers
	GetWorldTimerManager().ClearTimer(IdleTimerHandle);
	
	// Disable collision to prevent multiple hits
	ShipCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	if (DeathAnimation)
	{
		//if (GEngine)
		//{
		//	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("DeathAnimation found! Playing..."));
		//}
		//
		UAnimInstance* AnimInstance = BossMesh->GetAnimInstance();
		if (AnimInstance)
		{
			BossMesh->PlayAnimation(DeathAnimation, false); // Don't loop death animation
		}
		else
		{
			BossMesh->SetAnimation(DeathAnimation);
		}
		
		// Get animation duration and set timer to destroy boss
		float AnimDuration = DeathAnimation->GetPlayLength();
		//if (GEngine)
		//{
		//	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("Death animation duration: %.2f seconds"), AnimDuration));
		//}
		GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &ABoss_Z::DestroyBoss, AnimDuration, false);
	}
	else
	{
		//if (GEngine)
		//{
		//	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("No DeathAnimation found! Destroying immediately..."));
		//}
		// If no death animation, destroy immediately
		DestroyBoss();
	}
}

void ABoss_Z::DestroyBoss()
{
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Destroying Boss!"));
	//}
	
	Destroy();
}

void ABoss_Z::OnBossHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, TEXT("Boss collision detected!"));
	//}

	if (!OtherActor || OtherActor == this)
	{
		//if (GEngine)
		//{
		//	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Collision with self or null!"));
		//}
		return;
	}

	if (GEngine)
	{
		FString ActorName = OtherActor->GetName();
		FString ActorClass = OtherActor->GetClass()->GetName();
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("Collided with: %s (Class: %s)"), *ActorName, *ActorClass));
	}

	// Only process hit if not already in death state
	if (CurrentState != EBossState::Death)
	{
		// Check if it's a player projectile using multiple methods
		bool bIsProjectile = false;
		
		// Method 1: Direct class check
		if (OtherActor->IsA(AMobileSpaceProjectile::StaticClass()))
		{
			bIsProjectile = true;
			//if (GEngine)
			//{
			//	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Detected by IsA(AMobileSpaceProjectile)"));
			//}
		}
		
		// Method 2: Name check (backup)
		if (!bIsProjectile && OtherActor->GetName().Contains(TEXT("Projectile")))
		{
			bIsProjectile = true;
			//if (GEngine)
			//{
			//	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Detected by name containing 'Projectile'"));
			//}
		}
		
		// Method 3: Class name check (backup)
		if (!bIsProjectile && OtherActor->GetClass()->GetName().Contains(TEXT("MobileSpaceProjectile")))
		{
			bIsProjectile = true;
			//if (GEngine)
			//{
			//	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Magenta, TEXT("Detected by class name containing 'MobileSpaceProjectile'"));
			//}
		}
		
		if (bIsProjectile)
		{
			//if (GEngine)
			//{
			//	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("HIT BY PLAYER PROJECTILE! Starting death sequence..."));
			//}
			//
			// Destroy the projectile
			OtherActor->Destroy();
			
			// Start death phase
			StartDeathPhase();
		}
		//else
		//{
		//	if (GEngine)
		//	{
		//		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Not a recognized projectile"));
		//	}
		//}
	}
	//else
	//{
	//	if (GEngine)
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Purple, TEXT("Boss already in death state, ignoring collision"));
	//	}
	//}
}


