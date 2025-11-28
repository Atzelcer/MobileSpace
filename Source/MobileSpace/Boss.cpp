// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss.h"
#include "Engine/Engine.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "MobileSpaceProjectile.h"
#include "EngineUtils.h"
#include "Components/AudioComponent.h"
#include "HUDmain.h"
#include "WidgetMegaBoss.h"

ABoss::ABoss()
{
	PrimaryActorTick.bCanEverTick = true;

	BossMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BossMesh"));
	RootComponent = BossMesh;
	BossMesh->SetRelativeScale3D(FVector(0.8f, 0.8f, 0.8f));
	BossCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BossCollision"));
	BossCollision->SetupAttachment(BossMesh);
	BossCollision->SetBoxExtent(FVector(400.0f, 200.0f, 100.0f)); 

	MoveComp = CreateDefaultSubobject<UMoveComponent>(TEXT("MoveComponent"));
	MoveComp->Pattern = EArcadeMovement::BossMajesticArc;
	MoveComp->Speed = 150.0f; 
	MoveComp->Amplitude = 200.0f;
	MoveComp->Frequency = 0.5f; 

	TrailParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TrailParticles"));
	TrailParticleComponent->SetupAttachment(BossMesh);
	TrailParticleComponent->bAutoActivate = false; 
	TrailParticleComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	bTrailActiveOnSpawn = true;

	AttackComp = CreateDefaultSubobject<UAtackComponent>(TEXT("AttackComponent"));

	BossAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("BossAudioComponent"));
	BossAudioComponent->SetupAttachment(BossMesh);
	BossAudioComponent->bAutoActivate = false;



	CurrentHealth = BossHealth;
	FireRate = 2.0f;
	AttackPattern = EAtackPattern::Spread;

	EntranceHeight = 800.0f;
	EntranceSpeed = 300.0f;
	EntranceDuration = 3.0f;

	MainHUD = nullptr;

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

	
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{
		MainHUD = Cast<AHUDmain>(PC->GetHUD());
	}

	ShowBossHealthBar();
	if (TrailParticleComponent && TrailEffect)
	{
		TrailParticleComponent->SetTemplate(TrailEffect);
		TrailParticleComponent->SetRelativeLocation(TrailOffset);

		if (bTrailActiveOnSpawn)
		{
			GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
				{
					ActivateTrail();
				});
		}
	}

	StartEpicEntrance();
}void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!bIsEntering && bCanRotate) 
	{
		APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if (Player)
		{
			FVector PlayerLocation = Player->GetActorLocation();
			FVector BossLocation = GetActorLocation();
			FVector Direction = PlayerLocation - BossLocation;
			Direction.Z = 0.0f; 
			
			FRotator TargetRotation = Direction.Rotation();
			
			FRotator CurrentRotation = GetActorRotation();
			FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 2.0f);
			SetActorRotation(NewRotation);
		}
	}
}

void ABoss::AutoFire()
{
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
	CurrentHealth -= DamageAmount;

	UpdateBossHealthBar();

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

	DeactivateTrail();

	HideBossHealthBar();

	if (BossAudioComponent && BossAudioComponent->IsPlaying())
	{
		BossAudioComponent->Stop();
	}

	if (GetWorld())
	{
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


void ABoss::StartEpicEntrance()
{
	FinalPosition = GetActorLocation();
	
	StartPosition = FinalPosition + FVector(0.0f, 0.0f, EntranceHeight);
	SetActorLocation(StartPosition);
	
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
	
	GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, [this]()
	{
		bCanRotate = true;
	}, 1.0f, false); 
	
	if (BossCollision)
	{
		BossCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		BossCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	}
	
	FTimerHandle AttackTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &ABoss::AutoFire, FireRate, true);
	
		if (AppearanceEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), AppearanceEffect, 
			GetActorLocation(), FRotator::ZeroRotator, FVector(2.0f, 2.0f, 2.0f));
	}
}


void ABoss::ShowBossHealthBar()
{
	if (MainHUD && MainHUD->WidgetMegaBossClass)
	{
		if (!MainHUD->WidgetMegaBossInstance)
		{
			APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			if (PC)
			{
				MainHUD->WidgetMegaBossInstance = CreateWidget<UWidgetMegaBoss>(PC, MainHUD->WidgetMegaBossClass);
			}
		}

		if (MainHUD->WidgetMegaBossInstance)
		{
			MainHUD->WidgetMegaBossInstance->AddToViewport();
			UpdateBossHealthBar(); 
		}
	}
}

void ABoss::HideBossHealthBar()
{
	if (MainHUD && MainHUD->WidgetMegaBossInstance)
	{
		MainHUD->WidgetMegaBossInstance->RemoveFromViewport();
	}
}

void ABoss::UpdateBossHealthBar()
{
	if (MainHUD && MainHUD->WidgetMegaBossInstance)
	{
		int32 SafeCurrentHealth = FMath::Max(0, CurrentHealth);
		
		MainHUD->WidgetMegaBossInstance->UpdateBossLife(
			static_cast<float>(SafeCurrentHealth),
			static_cast<float>(BossHealth)
		);
	}
}

void ABoss::ActivateTrail()
{
	if (TrailParticleComponent && TrailEffect)
	{
		if (!TrailParticleComponent->IsActive())
		{
			TrailParticleComponent->Activate(true);

		}
	}
}

void ABoss::DeactivateTrail()
{
	if (TrailParticleComponent && TrailParticleComponent->IsActive())
	{
		TrailParticleComponent->Deactivate();

		if (GEngine)
		{
			
		}
	}
}
