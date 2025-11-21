#include "AtackComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"

UAtackComponent::UAtackComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    bCanFire = true;
    BurstCount = 0;
    ProjectileCycle = 0;
}

void UAtackComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UAtackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAtackComponent::EnableAttack(bool bEnable)
{
    bAttackEnabled = bEnable;
}

void UAtackComponent::SetPattern(EAttackPattern NewPattern)
{
    Pattern = NewPattern;
}

void UAtackComponent::SetProjectileClasses(TSubclassOf<AActor> A, TSubclassOf<AActor> B, TSubclassOf<AActor> C)
{
    ProjectileA = A;
    ProjectileB = B;
    ProjectileC = C;
}

void UAtackComponent::RequestFire()
{
    if (!bAttackEnabled) return;
    if (!bCanFire) return;

    bCanFire = false;
    ExecuteFirePattern();

    GetWorld()->GetTimerManager().SetTimer(TimerHandle_FireRate, this, &UAtackComponent::ResetFire, FireRate, false);
}

void UAtackComponent::ResetFire()
{
    bCanFire = true;
    BurstCount = 0;
}

void UAtackComponent::ExecuteFirePattern()
{
    switch (Pattern)
    {
    case EAttackPattern::Single:       FireSingle(); break;
    case EAttackPattern::Spread:       FireSpread(); break;
    case EAttackPattern::Burst:        FireBurst(); break;
    case EAttackPattern::TripleArc:    FireTripleArc(); break;
    case EAttackPattern::SniperShot:   FireSniperShot(); break;
    case EAttackPattern::Circular:     FireCircular(); break;
    default: break;
    }
}

void UAtackComponent::SpawnProjectile(TSubclassOf<AActor> ProjClass, const FVector& Loc, const FRotator& Rot)
{
    if (!ProjClass) return;
    if (!GetOwner()) return;

    FActorSpawnParameters Params;
    Params.Owner = GetOwner();
    GetWorld()->SpawnActor<AActor>(ProjClass, Loc, Rot, Params);

    if (FireSound)
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetOwner()->GetActorLocation());
}

void UAtackComponent::FireSingle()
{
    AActor* Owner = GetOwner();
    if (!Owner) return;

    FVector L = Owner->GetActorLocation() + Owner->GetActorForwardVector() * 120.f;
    FRotator R = Owner->GetActorForwardVector().Rotation();

    TSubclassOf<AActor> Proj = nullptr;

    switch (ProjectileCycle++ % 3)
    {
    case 0: Proj = ProjectileA; break;
    case 1: Proj = ProjectileB; break;
    case 2: Proj = ProjectileC; break;
    }

    SpawnProjectile(Proj, L, R);
}

void UAtackComponent::FireSpread()
{
    AActor* Owner = GetOwner();
    if (!Owner) return;

    FVector L = Owner->GetActorLocation() + Owner->GetActorForwardVector() * 120.f;
    FRotator BaseRot = Owner->GetActorForwardVector().Rotation();

    SpawnProjectile(ProjectileA, L, FRotator(BaseRot.Pitch, BaseRot.Yaw - SpreadAngle, BaseRot.Roll));
    SpawnProjectile(ProjectileB, L, BaseRot);
    SpawnProjectile(ProjectileC, L, FRotator(BaseRot.Pitch, BaseRot.Yaw + SpreadAngle, BaseRot.Roll));
}

void UAtackComponent::FireBurst()
{
    AActor* Owner = GetOwner();
    if (!Owner) return;

    FireSingle();
    FireSingle();
    FireSingle();
}

void UAtackComponent::FireTripleArc()
{
    AActor* Owner = GetOwner();
    if (!Owner) return;

    FVector L = Owner->GetActorLocation() + Owner->GetActorForwardVector() * 120.f;
    FRotator BaseRot = Owner->GetActorForwardVector().Rotation();

    SpawnProjectile(ProjectileA, L, FRotator(BaseRot.Pitch, BaseRot.Yaw - 10.f, BaseRot.Roll));
    SpawnProjectile(ProjectileB, L, BaseRot);
    SpawnProjectile(ProjectileC, L, FRotator(BaseRot.Pitch, BaseRot.Yaw + 10.f, BaseRot.Roll));
}

void UAtackComponent::FireSniperShot()
{
    AActor* Owner = GetOwner();
    if (!Owner) return;

    FVector L = Owner->GetActorLocation() + Owner->GetActorForwardVector() * 150.f;
    FRotator R = Owner->GetActorForwardVector().Rotation();

    SpawnProjectile(ProjectileC, L, R);
}

void UAtackComponent::FireCircular()
{
    AActor* Owner = GetOwner();
    if (!Owner) return;

    FVector L = Owner->GetActorLocation();

    for (int32 i = 0; i < 12; i++)
    {
        float Yaw = i * 30.f;
        SpawnProjectile(ProjectileA, L, FRotator(0.f, Yaw, 0.f));
    }
}
