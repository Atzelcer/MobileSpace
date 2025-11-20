#include "Boss_Z.h"
#include "MobileSpaceProjectile.h"
#include "MobileSpacePawn.h"
#include "HUDmain.h"
#include "WidgetMegaBoss.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"

// =============================================================
// CONSTRUCTOR
// =============================================================
ABoss_Z::ABoss_Z()
{
	PrimaryActorTick.bCanEverTick = true;

	ShipCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ShipCollision"));
	ShipCollision->SetBoxExtent(FVector(300.f, 300.f, 300.f));
	ShipCollision->SetGenerateOverlapEvents(true);
	ShipCollision->SetupAttachment(RootComponent);

	ShipCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ShipCollision->SetCollisionObjectType(ECC_Pawn);
	ShipCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	ShipCollision->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	ShipCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	BossMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BossMesh"));
	BossMesh->SetupAttachment(ShipCollision);
	BossMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MoveComp = CreateDefaultSubobject<UMoveComponent>(TEXT("MoveComp"));
}

// =============================================================
// BEGINPLAY
// =============================================================
void ABoss_Z::BeginPlay()
{
	Super::BeginPlay();

	ShipCollision->OnComponentBeginOverlap.AddDynamic(this, &ABoss_Z::OnBossHit);

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC) return;

	HUD = Cast<AHUDmain>(PC->GetHUD());
	if (HUD)
	{
		HUD->MostrarMegaBoss();

		if (HUD->WidgetMegaBossInstance)
			HUD->WidgetMegaBossInstance->UpdateBossLife(MaxHealth, MaxHealth);
	}

	CurrentHealth = MaxHealth;

	SpawnSequence();
}

// =============================================================
// TICK
// =============================================================
void ABoss_Z::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// =============================================================
// NOTIFY OVERLAP (ACTOR GENERAL)
// =============================================================
void ABoss_Z::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (bIsDead || !OtherActor) return;

	// --- daño por proyectil ---
	if (AMobileSpaceProjectile* Projectile = Cast<AMobileSpaceProjectile>(OtherActor))
	{
		CurrentHealth -= 10.f;
		Projectile->Destroy();
	}

	// --- daño por colisión con la nave ---
	if (AMobileSpacePawn* Player = Cast<AMobileSpacePawn>(OtherActor))
	{
		CurrentHealth -= 20.f;
	}

	// actualizar HUD
	if (HUD && HUD->WidgetMegaBossInstance)
		HUD->WidgetMegaBossInstance->UpdateBossLife(CurrentHealth, MaxHealth);

	if (CurrentHealth <= 0.f)
		DeathSequence();
}

// =============================================================
// OVERLAP DEL COMPONENTE DE COLISIÓN
// =============================================================
void ABoss_Z::OnBossHit(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (bIsDead || !OtherActor) return;

	if (AMobileSpaceProjectile* Projectile = Cast<AMobileSpaceProjectile>(OtherActor))
	{
		CurrentHealth -= 10.f;
		Projectile->Destroy();
	}

	if (HUD && HUD->WidgetMegaBossInstance)
		HUD->WidgetMegaBossInstance->UpdateBossLife(CurrentHealth, MaxHealth);

	if (CurrentHealth <= 0.f)
		DeathSequence();
}

// =============================================================
// SPAWN EFFECT
// =============================================================
void ABoss_Z::SpawnSequence()
{
	if (SpawnParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			SpawnParticle,
			GetActorLocation(),
			GetActorRotation()
		);
	}
}

// =============================================================
// DEATH SEQUENCE
// =============================================================
void ABoss_Z::DeathSequence()
{
	if (bIsDead) return;

	bIsDead = true;

	if (DeathParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			DeathParticle,
			GetActorLocation(),
			GetActorRotation()
		);
	}

	if (HUD)
		HUD->OcultarMegaBoss();

	Destroy();
}

// =============================================================
// ATAQUE DEL BOSS (placeholder)
// =============================================================
void ABoss_Z::DispararAtaque()
{
}
