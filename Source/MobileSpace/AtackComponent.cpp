// Fill out your copyright notice in the Description page of Project Settings.


#include "AtackComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "Sound/SoundBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Projectile_A.h"  
#include "Projectile_B.h"  
#include "Projectile_C.h"  

UAtackComponent::UAtackComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    bCanFire = true;

    static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("SoundWave'/Game/GoodFXLevelUp/SFX/Sound_Wave/A_GFXLU_transitions---whoosh-ufo-power-down.A_GFXLU_transitions---whoosh-ufo-power-down'"));
    if (FireAudio.Succeeded())
    {
        FireSound = FireAudio.Object;
    }
    
}

void UAtackComponent::Fire(EAtackPattern Pattern)
{
    if (!bCanFire) return;

    switch (Pattern)
    {
    case EAtackPattern::Single:
        FireSingle();
        break;
    case EAtackPattern::Spread:
        FireSpread();
        break;
    case EAtackPattern::Burst:
        FireBurst();
        break;
    case EAtackPattern::BossCircular:
        FireBossCircular();
        break;
    case EAtackPattern::BossSpiral:
        FireBossSpiral();
        break;
    case EAtackPattern::BossWaveBarrage:
        FireBossWaveBarrage();
        break;
    case EAtackPattern::BossTargeted:
        FireBossTargeted();
        break;
    case EAtackPattern::BossEpicFinal:
        FireBossEpicFinal();
        break;
    }

    bCanFire = false;
    GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &UAtackComponent::OnFireTimerExpired, FireRate, false);
}


void UAtackComponent::BeginPlay()
{
    Super::BeginPlay();

 

}


void UAtackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    
}

void UAtackComponent::OnFireTimerExpired()
{
    bCanFire = true;
}

void UAtackComponent::FireSingle()
{
    AActor* Owner = GetOwner();
    if (!Owner) return;

    FVector SpawnLocation = Owner->GetActorLocation() + Owner->GetActorForwardVector() * 80.0f;
    FRotator SpawnRotation = Owner->GetActorForwardVector().Rotation();

    UWorld* World = GetWorld();
    if (!World) return;

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = Owner;

    switch (ProjectileCycle++ % 3)
    {
    case 0:
        World->SpawnActor<AProjectile_A>(AProjectile_A::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
        break;
    case 1:
        UE_LOG(LogTemp, Warning, TEXT("Spawned Projectile_B"));
        break;
    case 2:
        World->SpawnActor<AProjectile_C>(AProjectile_C::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
        break;
    }

    if (FireSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, Owner->GetActorLocation());
    }
}



void UAtackComponent::FireSpread()
{
    AActor* Owner = GetOwner();
    if (!Owner) return;

    UWorld* World = GetWorld();
    if (!World) return;

    FVector SpawnLocation = Owner->GetActorLocation() + Owner->GetActorForwardVector() * 80.0f;
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = Owner;

    FRotator RotA = Owner->GetActorForwardVector().Rotation();
    RotA.Yaw -= 15.0f;
    World->SpawnActor<AProjectile_A>(AProjectile_A::StaticClass(), SpawnLocation, RotA, SpawnParams);

    FRotator RotB = Owner->GetActorForwardVector().Rotation();
    World->SpawnActor<AProjectile_B>(AProjectile_B::StaticClass(), SpawnLocation, RotB, SpawnParams);

    FRotator RotC = Owner->GetActorForwardVector().Rotation();
    RotC.Yaw += 15.0f;
    World->SpawnActor<AProjectile_C>(AProjectile_C::StaticClass(), SpawnLocation, RotC, SpawnParams);

    if (FireSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, Owner->GetActorLocation());
    }

}

void UAtackComponent::FireBurst()
{
    FireSingle();
    FireSingle();
    FireSingle();

    if (FireSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetOwner()->GetActorLocation());
    }
}


void UAtackComponent::FireBossCircular()
{
    AActor* Owner = GetOwner();
    if (!Owner) return;

    UWorld* World = GetWorld();
    if (!World) return;

    FVector SpawnLocation = Owner->GetActorLocation() + FVector(0, 0, 0);
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = Owner;

    int32 NumShots = 8;
    for (int32 i = 0; i < NumShots; i++)
    {
        float Angle = (360.0f / NumShots) * i;
        FRotator SpawnRotation = FRotator(0.0f, Angle, 0.0f);

        switch (i % 3)
        {
        case 0:
            World->SpawnActor<AProjectile_A>(AProjectile_A::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
            break;
        case 1:
            World->SpawnActor<AProjectile_B>(AProjectile_B::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
            break;
        case 2:
            World->SpawnActor<AProjectile_C>(AProjectile_C::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
            break;
        }
    }

    if (FireSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, Owner->GetActorLocation());
    }

}

void UAtackComponent::FireBossSpiral()
{
    AActor* Owner = GetOwner();
    if (!Owner) return;

    UWorld* World = GetWorld();
    if (!World) return;

    FVector SpawnLocation = Owner->GetActorLocation();
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = Owner;

    int32 NumShots = 5;
    for (int32 i = 0; i < NumShots; i++)
    {
        float AngleOffset = (360.0f / NumShots) * i;
        float FinalAngle = SpiralAngle + AngleOffset;
        FRotator SpawnRotation = FRotator(0.0f, FinalAngle, 0.0f);

        World->SpawnActor<AProjectile_A>(AProjectile_A::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
    }

    SpiralAngle += 25.0f; 
    if (SpiralAngle >= 360.0f) SpiralAngle = 0.0f;

    if (FireSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, Owner->GetActorLocation());
    }

}

void UAtackComponent::FireBossWaveBarrage()
{
    AActor* Owner = GetOwner();
    if (!Owner) return;

    UWorld* World = GetWorld();
    if (!World) return;

    FVector SpawnLocation = Owner->GetActorLocation();
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = Owner;

    int32 NumWaves = 3;
    int32 ShotsPerWave = 5;

    for (int32 wave = 0; wave < NumWaves; wave++)
    {
        for (int32 shot = 0; shot < ShotsPerWave; shot++)
        {
            float BaseAngle = -60.0f + (120.0f / (ShotsPerWave - 1)) * shot;
            float WaveOffset = wave * 10.0f; 
            float FinalAngle = BaseAngle + WaveOffset;

            FRotator SpawnRotation = FRotator(0.0f, FinalAngle, 0.0f);

            switch (wave)
            {
            case 0:
                World->SpawnActor<AProjectile_A>(AProjectile_A::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
                break;
            case 1:
                World->SpawnActor<AProjectile_B>(AProjectile_B::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
                break;
            case 2:
                World->SpawnActor<AProjectile_C>(AProjectile_C::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
                break;
            }
        }
    }

    if (FireSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, Owner->GetActorLocation());
    }

}

void UAtackComponent::FireBossTargeted()
{
    AActor* Owner = GetOwner();
    if (!Owner) return;

    UWorld* World = GetWorld();
    if (!World) return;

    APawn* Player = UGameplayStatics::GetPlayerPawn(World, 0);
    if (!Player) return;

    FVector SpawnLocation = Owner->GetActorLocation();
    FVector PlayerLocation = Player->GetActorLocation();
    FVector Direction = (PlayerLocation - SpawnLocation).GetSafeNormal();

    FRotator TargetRotation = Direction.Rotation();
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = Owner;

    World->SpawnActor<AProjectile_B>(AProjectile_B::StaticClass(), SpawnLocation, TargetRotation, SpawnParams);

    for (int32 i = 0; i < 4; i++)
    {
        float Spread = (i - 2) * 8.0f;
        FRotator SpreadRotation = TargetRotation;
        SpreadRotation.Yaw += Spread;

        World->SpawnActor<AProjectile_A>(AProjectile_A::StaticClass(), SpawnLocation, SpreadRotation, SpawnParams);
    }

    if (FireSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, Owner->GetActorLocation());
    }

}

void UAtackComponent::FireBossEpicFinal()
{
    AActor* Owner = GetOwner();
    if (!Owner) return;

    UWorld* World = GetWorld();
    if (!World) return;

    FVector SpawnLocation = Owner->GetActorLocation();
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = Owner;


    for (int32 i = 0; i < 12; i++)
    {
        float Angle = (360.0f / 12) * i;
        FRotator SpawnRotation = FRotator(0.0f, Angle, 0.0f);
        World->SpawnActor<AProjectile_C>(AProjectile_C::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
    }

    for (int32 spiral = 0; spiral < 2; spiral++)
    {
        for (int32 i = 0; i < 8; i++)
        {
            float AngleOffset = (360.0f / 8) * i;
            float SpiralOffset = spiral * 180.0f; 
            float FinalAngle = SpiralAngle + AngleOffset + SpiralOffset;
            FRotator SpawnRotation = FRotator(0.0f, FinalAngle, 0.0f);

            World->SpawnActor<AProjectile_A>(AProjectile_A::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
        }
    }

    APawn* Player = UGameplayStatics::GetPlayerPawn(World, 0);
    if (Player)
    {
        FVector PlayerLocation = Player->GetActorLocation();
        FVector Direction = (PlayerLocation - SpawnLocation).GetSafeNormal();
        FRotator TargetRotation = Direction.Rotation();

        for (int32 i = 0; i < 6; i++)
        {
            float Spread = (i - 3) * 5.0f;
            FRotator SpreadRotation = TargetRotation;
            SpreadRotation.Yaw += Spread;
            World->SpawnActor<AProjectile_B>(AProjectile_B::StaticClass(), SpawnLocation, SpreadRotation, SpawnParams);
        }
    }

    SpiralAngle += 30.0f;
    if (SpiralAngle >= 360.0f) SpiralAngle = 0.0f;

    if (FireSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, Owner->GetActorLocation());
    }

}