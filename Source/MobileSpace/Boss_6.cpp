#include "Boss_6.h"
#include "UObject/ConstructorHelpers.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundBase.h"
#include "Engine/World.h"
#include "TimerManager.h"

ABoss_6::ABoss_6()
{
    PrimaryActorTick.bCanEverTick = true;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> BossMeshAsset(
        TEXT("StaticMesh'/Game/BOSSES/SM_CF2_Shuttle.SM_CF2_Shuttle'"));

    if (BossMeshAsset.Succeeded() && BossMesh)
    {
        BossMesh->SetStaticMesh(BossMeshAsset.Object);
        BossMesh->SetRelativeScale3D(FVector(1.4f, 1.4f, 1.4f));
    }

    BossHealth = 5000;
    CurrentHealth = BossHealth;
    FireRate = 0.6f;
    AttackPattern = EAtackPattern::BossSpiral;

    static ConstructorHelpers::FObjectFinder<USoundBase> AppearanceSoundAsset(
        TEXT("SoundWave'/Game/BOSS_SOUNDS/METAMO.METAMO'"));
    if (AppearanceSoundAsset.Succeeded())
        AppearanceSound = AppearanceSoundAsset.Object;

    static ConstructorHelpers::FObjectFinder<USoundBase> DestructionSoundAsset(
        TEXT("SoundWave'/Game/MFK/Sounds/Explode/A_Explode_06.A_Explode_06'"));
    if (DestructionSoundAsset.Succeeded())
        DestructionSound = DestructionSoundAsset.Object;

    static ConstructorHelpers::FObjectFinder<UParticleSystem> AppearanceEffectAsset(
        TEXT("ParticleSystem'/Game/MFK/Particles/Expanders/Par_ExpFire_01_Rain.Par_ExpFire_01_Rain'"));
    if (AppearanceEffectAsset.Succeeded())
        AppearanceEffect = AppearanceEffectAsset.Object;

    static ConstructorHelpers::FObjectFinder<UParticleSystem> DestructionEffectAsset(
        TEXT("ParticleSystem'/Game/MFK/Particles/Expanders/Par_ExpFire_01_Rain.Par_ExpFire_01_Rain'"));
    if (DestructionEffectAsset.Succeeded())
        DestructionEffect = DestructionEffectAsset.Object;

    DestructionEffectScale = FVector(10.0f, 10.0f, 10.0f);

    if (MoveComp)
    {
        MoveComp->Pattern = EArcadeMovement::BossSlowSweep;
        MoveComp->Speed = 60.0f;
        MoveComp->Amplitude = 150.0f;
        MoveComp->Frequency = 0.2f;
    }

    if (BossCollision)
    {
        BossCollision->SetBoxExtent(FVector(1020.0f, 600.0f, 400.0f));
    }

    bEpicPhase = false;
}

void ABoss_6::BeginPlay()
{
    Super::BeginPlay();

    if (GetWorld())
        GetWorld()->GetTimerManager().SetTimer(SpecialAttackTimerHandle, this, &ABoss_6::SwitchAttackPhase, 6.0f, false);
}

void ABoss_6::SwitchAttackPhase()
{
    if (!IsValid(this) || IsPendingKill() || !GetWorld())
        return;

    if (!bEpicPhase)
    {
        bEpicPhase = true;
        AttackPattern = EAtackPattern::BossEpicFinal;
        FireRate = 0.3f;

        if (GetWorld())
        {
            GetWorld()->GetTimerManager().SetTimer(EpicPhaseTimerHandle, [this]()
                {
                    if (!IsValid(this) || IsPendingKill() || !GetWorld())
                        return;

                    bEpicPhase = false;
                    AttackPattern = EAtackPattern::Single;
                    FireRate = 10.0f;

                    if (GetWorld())
                        GetWorld()->GetTimerManager().SetTimer(SpecialAttackTimerHandle, this, &ABoss_6::SwitchAttackPhase, 6.0f, false);

                }, 3.0f, false);
        }
    }
}

void ABoss_6::Destroyed()
{
    Super::Destroyed();

    if (GetWorld())
        GetWorld()->GetTimerManager().ClearAllTimersForObject(this);


}
