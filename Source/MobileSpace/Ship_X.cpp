#include "Ship_X.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "MoveComponent.h"
#include "AtackComponent.h"
#include "MobileSpaceProjectile.h"
#include "MegaCapsula.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"

AShip_X::AShip_X()
{
    PrimaryActorTick.bCanEverTick = true;

    ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
    RootComponent = ShipMesh;

    ShipCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ShipCollision"));
    ShipCollision->SetupAttachment(ShipMesh);
    ShipCollision->SetBoxExtent(FVector(420.f, 455.f, 200.f));
    ShipCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    ShipCollision->SetCollisionResponseToAllChannels(ECR_Overlap);
    ShipCollision->OnComponentBeginOverlap.AddDynamic(this, &AShip_X::OnShipHit);

    MoveComp = CreateDefaultSubobject<UMoveComponent>(TEXT("MoveComp"));
    AttackComp = CreateDefaultSubobject<UAtackComponent>(TEXT("AttackComponent"));

    TrailEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TrailEffect"));
    TrailEffect->SetupAttachment(ShipMesh);

    static ConstructorHelpers::FObjectFinder<UParticleSystem> TrailFX(TEXT("ParticleSystem'/Game/VFXSeries1/Particles/Tails/P_Plasmatrail01.P_Plasmatrail01'"));
    if (TrailFX.Succeeded())
    {
        TrailEffect->SetTemplate(TrailFX.Object);
        TrailEffect->SetWorldScale3D(FVector(2.5f, 2.5f, 2.5f));
    }

    static ConstructorHelpers::FObjectFinder<USoundBase> ExplosionSound(TEXT("SoundWave'/Game/StarterContent/Audio/Explosion01.Explosion01'"));
    if (ExplosionSound.Succeeded())
        SonidoExplosion = ExplosionSound.Object;

    static ConstructorHelpers::FObjectFinder<UParticleSystem> ExplosionFX(TEXT("ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_explosion.P_ky_explosion'"));
    if (ExplosionFX.Succeeded())
        DestructionEffect = ExplosionFX.Object;

    DestructionEffectScale = FVector(3.f, 3.f, 3.f);

    ProbabilidadSpawnCapsula = 23;

    Tipo = ENaveTipo::Roja;
    ShipRole = EShipRole::Normal;

    MovementPattern = EArcadeMovement::Wave;
    AttackPattern = EAttackPattern::Single;


    bFireEnabled = false;
    bUseFormation = false;
    bPuedeAtacar = true;

    FormationAnchor = FVector::ZeroVector;
    FormationOffset = FVector::ZeroVector;
}

void AShip_X::BeginPlay()
{
    Super::BeginPlay();
    if (MoveComp)
        MoveComp->Pattern = MovementPattern;
}

void AShip_X::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FVector Loc = GetActorLocation();

    // ============================================================
    // TELETRANSPORTACIÓN AL SALIR POR EL BORDE IZQUIERDO
    // ============================================================
    const float MinX = -1400.f;
    const float MaxX = 1400.f;
    const float MinY = -3000.f;
    const float MaxY = 3000.f;

    if (Loc.X < MinX)
    {
        Loc.X = MaxX + 400.f;
        Loc.Y = FMath::FRandRange(MinY, MaxY);
        SetActorLocation(Loc);
    }

    // ============================================================
    // MOVIMIENTO POR FORMACIÓN
    // ============================================================
    if (bUseFormation)
    {
        FVector Target = FormationAnchor + FormationOffset;
        FVector NewPos = FMath::VInterpTo(Loc, Target, DeltaTime, 2.5f);
        SetActorLocation(NewPos);
    }

    // ============================================================
    // MOVIMIENTO PERSEGUIDOR
    // ============================================================
    if (ShipRole == EShipRole::Perseguidor)
    {
        APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
        if (Player)
        {
            FVector Dir = (Player->GetActorLocation() - GetActorLocation()).GetSafeNormal();
            SetActorRotation(Dir.Rotation());

            FVector Forward = GetActorForwardVector() * 500.f * DeltaTime;
            AddActorWorldOffset(Forward, true);
        }
    }

    // ============================================================
    // ATAQUE AUTOMÁTICO
    // ============================================================
    if (bFireEnabled)
        FireIfReady();
}


void AShip_X::SetMovement(EArcadeMovement NewPattern)
{
    MovementPattern = NewPattern;
    if (MoveComp)
        MoveComp->Pattern = NewPattern;
}

void AShip_X::SetAttackPattern(EAttackPattern NewPattern)
{
    AttackPattern = NewPattern;
}

void AShip_X::EnableFire(bool Enabled)
{
    bFireEnabled = Enabled;
}

void AShip_X::SetRole(EShipRole NewRole)
{
    ShipRole = NewRole;
}

void AShip_X::SetFormation(EFormationType Type, FVector Offset, FVector Anchor)
{
    FormationOffset = Offset;
    FormationAnchor = Anchor;
    bUseFormation = true;
}

void AShip_X::FireIfReady()
{
    if (!AttackComp) return;
    if (!bPuedeAtacar) return;

    AttackComp->RequestFire();
}

void AShip_X::OnShipHit(UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->IsA(AMobileSpaceProjectile::StaticClass()))
        HandleDestruction();
}

void AShip_X::HandleDestruction()
{
    if (DestructionEffect)
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DestructionEffect, GetActorLocation(), GetActorRotation(), DestructionEffectScale);

    if (SonidoExplosion)
        UGameplayStatics::PlaySoundAtLocation(this, SonidoExplosion, GetActorLocation());

    IntentarSpawnCapsula();
    Destroy();
}

void AShip_X::IntentarSpawnCapsula()
{
    UWorld* World = GetWorld();
    if (!World) return;

    int32 RandomValue = FMath::RandRange(1, 100);
    if (RandomValue <= ProbabilidadSpawnCapsula)
        World->SpawnActor<AMegaCapsula>(AMegaCapsula::StaticClass(), GetActorLocation(), GetActorRotation());
}

void AShip_X::DestruirNave()
{
    HandleDestruction();
}

bool AShip_X::GetAttackState() const
{
    return bPuedeAtacar;
}

void AShip_X::SetAttackState(bool Estado)
{
    bPuedeAtacar = Estado;
}

void AShip_X::ForzarDisparo()
{
    if (!AttackComp) return;
    if (!bPuedeAtacar) return;

    AttackComp->RequestFire();
}
