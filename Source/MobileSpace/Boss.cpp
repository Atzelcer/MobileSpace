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

	SetActorRotation(FRotator(0.0f, -180.0f, 0.0f));

	if (BossCollision)
	{
		BossCollision->OnComponentBeginOverlap.AddDynamic(this, &ABoss::OnBossHit);
	}

	StartEpicEntrance();
}

void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
		TakeDamage(100);
		OtherActor->Destroy();
	}
}

void ABoss::TakeDamage(int32 DamageAmount)
{
	CurrentHealth -= DamageAmount;

	if (CurrentHealth <= 0)
	{
		DestroyBoss();
	}
}

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
	
	// Timer para actualizar la animación de entrada
	GetWorld()->GetTimerManager().SetTimer(EntranceTimerHandle, this, &ABoss::UpdateEntrance, 0.016f, true); // 60 FPS
	
	UE_LOG(LogTemp, Warning, TEXT("🎬 ENTRADA ÉPICA INICIADA - Bajando desde las alturas! 🎬"));
}

void ABoss::UpdateEntrance()
{
	EntranceTimeElapsed += 0.016f; // 60 FPS
	
	if (EntranceTimeElapsed >= EntranceDuration)
	{
		FinishEntrance();
		return;
	}
	
	// Interpolación suave desde posición inicial hasta final
	float Alpha = EntranceTimeElapsed / EntranceDuration;
	
	// Usar curva de suavizado para entrada más dramática
	float SmoothedAlpha = FMath::SmoothStep(0.0f, 1.0f, Alpha);
	
	// Calcular posición actual
	FVector CurrentPos = FMath::Lerp(StartPosition, FinalPosition, SmoothedAlpha);
	SetActorLocation(CurrentPos);
	
	// Efecto de temblor durante la bajada (opcional)
	if (Alpha > 0.5f) // Solo en la segunda mitad
	{
		FVector Shake = FVector(
			FMath::RandRange(-10.0f, 10.0f),
			FMath::RandRange(-10.0f, 10.0f),
			0.0f
		) * (1.0f - Alpha); // Menos temblor cerca del final
		
		SetActorLocation(CurrentPos + Shake);
	}
}

void ABoss::FinishEntrance()
{
	// Detener timer de entrada
	GetWorld()->GetTimerManager().ClearTimer(EntranceTimerHandle);
	
	// Asegurar posición final exacta
	SetActorLocation(FinalPosition);
	
	// Terminar entrada épica
	bIsEntering = false;
	
	// ===== INICIAR COMBATE NORMAL =====
	if (BossCollision)
	{
		BossCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		BossCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	}
	
	GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &ABoss::AutoFire, FireRate, true);
	
	// Efectos de aterrizaje
	if (AppearanceEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), AppearanceEffect, 
			GetActorLocation(), FRotator::ZeroRotator, FVector(2.0f, 2.0f, 2.0f));
	}
}

