// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss.h"
#include "Engine/Engine.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "MobileSpaceProjectile.h"
#include "EngineUtils.h"
#include "Components/AudioComponent.h"

// Sets default values
ABoss::ABoss()
{
	PrimaryActorTick.bCanEverTick = true;

	// ===== CONFIGURACIÓN DE COMPONENTES =====
	// Malla principal del jefe
	BossMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BossMesh"));
	RootComponent = BossMesh;
	BossMesh->SetRelativeScale3D(FVector(0.8f, 0.8f, 0.8f)); // Escala para jefes

	// Colisión
	BossCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BossCollision"));
	BossCollision->SetupAttachment(BossMesh);
	BossCollision->SetBoxExtent(FVector(200.0f, 200.0f, 100.0f)); // Hitbox grande para jefes

	// Componente de movimiento - patrón majestuoso para jefes
	MoveComp = CreateDefaultSubobject<UMoveComponent>(TEXT("MoveComponent"));
	MoveComp->Pattern = EArcadeMovement::BossMajesticArc;
	MoveComp->Speed = 150.0f; // Movimiento lento y elegante
	MoveComp->Amplitude = 200.0f;
	MoveComp->Frequency = 0.5f; // Muy lento

	

	// Componente de ataque
	AttackComp = CreateDefaultSubobject<UAtackComponent>(TEXT("AttackComponent"));

	// Componente de audio
	BossAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("BossAudioComponent"));
	BossAudioComponent->SetupAttachment(BossMesh);
	BossAudioComponent->bAutoActivate = false;

	// ===== CONFIGURACIÓN DE COMBATE =====
	CurrentHealth = BossHealth;
	FireRate = 2.0f;
	AttackPattern = EAtackPattern::Spread;

	// ===== CONFIGURACIÓN DE ENTRADA ÉPICA =====
	EntranceHeight = 800.0f;
	EntranceSpeed = 300.0f;
	EntranceDuration = 3.0f;

	BossCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BossCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
}

void ABoss::BeginPlay()
{
	Super::BeginPlay();

	if (BossCollision)
	{
		BossCollision->OnComponentBeginOverlap.AddDynamic(this, &ABoss::OnBossHit);
	}

	StartEpicEntrance();
}

void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Sistema de seguimiento del jugador para todos los jefes
	if (!bIsEntering && bCanRotate) // Solo seguir si terminó entrada y puede rotar
	{
		// Buscar al jugador
		APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if (Player)
		{
			// Calcular dirección hacia el jugador
			FVector PlayerLocation = Player->GetActorLocation();
			FVector BossLocation = GetActorLocation();
			FVector Direction = PlayerLocation - BossLocation;
			Direction.Z = 0.0f; // Mantener rotación solo en el plano XY
			
			// Convertir dirección a rotación
			FRotator TargetRotation = Direction.Rotation();
			
			// Rotar suavemente hacia el jugador
			FRotator CurrentRotation = GetActorRotation();
			FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 2.0f);
			SetActorRotation(NewRotation);
		}
	}
}

void ABoss::AutoFire()
{
	// No disparar durante la entrada épica
	if (bIsEntering) return;

	if (AttackComp)
	{
		AttackComp->Fire(AttackPattern);
	}
}

void ABoss::OnBossHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->IsA(AMobileSpaceProjectile::StaticClass()))
	{
		ApplyBossDamage(100);
		OtherActor->Destroy();
	}
}

void ABoss::ApplyBossDamage(int32 DamageAmount)
{
	// Debug: mostrar daño y vida actual
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red,
			FString::Printf(TEXT("Boss recibe %d daño. Vida: %d/%d"), DamageAmount, CurrentHealth - DamageAmount, BossHealth));
	}

	CurrentHealth -= DamageAmount;

	if (CurrentHealth <= 0)
	{
		DestroyBoss();
	}
}

//void ABoss::TakeDamage(int32 DamageAmount)
//{
//	// Debug: mostrar daño y vida actual
//	if (GEngine)
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, 
//			FString::Printf(TEXT("Boss recibe %d daño. Vida: %d/%d"), DamageAmount, CurrentHealth - DamageAmount, BossHealth));
//	}
//	
//	CurrentHealth -= DamageAmount;
//
//	if (CurrentHealth <= 0)
//	{
//		DestroyBoss();
//	}
//}

void ABoss::DestroyBoss()
{
	GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(SpecialAttackTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(EntranceTimerHandle);

	// ===== DETENER SONIDO DEL BOSS =====
	if (BossAudioComponent && BossAudioComponent->IsPlaying())
	{
		BossAudioComponent->Stop();
	}

	// Detener toda la música y sonidos del juego
	if (GetWorld())
	{
		// Método más agresivo: detener TODOS los AudioComponents
		for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			AActor* Actor = *ActorItr;
			if (Actor)
			{
				TArray<UAudioComponent*> AudioComponents;
				Actor->GetComponents<UAudioComponent>(AudioComponents);
				for (UAudioComponent* AudioComp : AudioComponents)
				{
					if (AudioComp && AudioComp->IsPlaying())
					{
						AudioComp->Stop();
					}
				}
			}
		}
		
		// También detener cualquier sonido 2D activo
		UGameplayStatics::SetBaseSoundMix(GetWorld(), nullptr);
	}

	if (DestructionEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DestructionEffect, 
			GetActorLocation(), FRotator::ZeroRotator, DestructionEffectScale);
	}

	if (DestructionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), DestructionSound, GetActorLocation());
	}

	Destroy();
}

// ===== IMPLEMENTACIÓN DE ENTRADA ÉPICA =====

void ABoss::StartEpicEntrance()
{
	// Guardar posición final donde debe quedar el jefe
	FinalPosition = GetActorLocation();
	
	// Colocar el jefe arriba para que baje
	StartPosition = FinalPosition + FVector(0.0f, 0.0f, EntranceHeight);
	SetActorLocation(StartPosition);
	
	// Inicializar variables de entrada
	bIsEntering = true;
	EntranceTimeElapsed = 0.0f;
	
	if (AppearanceSound && BossAudioComponent)
	{
		BossAudioComponent->SetSound(AppearanceSound);
		BossAudioComponent->Play();
	}
	
	if (AppearanceEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), AppearanceEffect, 
			GetActorLocation(), FRotator::ZeroRotator, FVector(1.5f, 1.5f, 1.5f));
	}
	
	GetWorld()->GetTimerManager().SetTimer(EntranceTimerHandle, this, &ABoss::UpdateEntrance, 0.016f, true);
	
}

void ABoss::UpdateEntrance()
{
	EntranceTimeElapsed += 0.016f; 
	
	if (EntranceTimeElapsed >= EntranceDuration)
	{
		FinishEntrance();
		return;
	}
	
	float Alpha = EntranceTimeElapsed / EntranceDuration;
	
	float SmoothedAlpha = FMath::SmoothStep(0.0f, 1.0f, Alpha);
	
	FVector CurrentPos = FMath::Lerp(StartPosition, FinalPosition, SmoothedAlpha);
	SetActorLocation(CurrentPos);
}


void ABoss::FinishEntrance()
{
	GetWorld()->GetTimerManager().ClearTimer(EntranceTimerHandle);
	
	SetActorLocation(FinalPosition);
	
	bIsEntering = false;
	
	// Activar rotación después de un pequeño delay para evitar shake
	GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, [this]()
	{
		bCanRotate = true;
	}, 1.0f, false); // 1 segundo de delay
	
	if (BossCollision)
	{
		BossCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		BossCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	}
	
	// Timer para disparo automático (usar otro timer diferente)
	FTimerHandle AttackTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &ABoss::AutoFire, FireRate, true);
	
	if (AppearanceEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), AppearanceEffect, 
			GetActorLocation(), FRotator::ZeroRotator, FVector(2.0f, 2.0f, 2.0f));
	}
}

