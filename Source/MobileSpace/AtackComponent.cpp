// Fill out your copyright notice in the Description page of Project Settings.


#include "AtackComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "Sound/SoundBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Projectile_A.h"  // ← Include directo
#include "Projectile_B.h"  // ← Include directo
#include "Projectile_C.h"  // ← Include directo
// Sets default values for this component's properties
UAtackComponent::UAtackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bCanFire = true;

    static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("SoundWave'/Game/Free_Sounds_Pack/wav/Explosion_Medium_2-1.Explosion_Medium_2-1'"));
    if (FireAudio.Succeeded())
    {
        FireSound = FireAudio.Object;
    }
	// ...
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
    // ===== NUEVOS PATRONES PARA JEFES =====
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


// Called when the game starts
void UAtackComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAtackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
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

    // Alterna entre los 3 proyectiles
    switch (ProjectileCycle++ % 3)
    {
    case 0:
        World->SpawnActor<AProjectile_A>(AProjectile_A::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
        //UE_LOG(LogTemp, Warning, TEXT("Spawned Projectile_A"));
        break;
    case 1:
        //World->SpawnActor<AProjectile_B>(AProjectile_B::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
        UE_LOG(LogTemp, Warning, TEXT("Spawned Projectile_B"));
        break;
    case 2:
        World->SpawnActor<AProjectile_C>(AProjectile_C::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
        //UE_LOG(LogTemp, Warning, TEXT("Spawned Projectile_C"));
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

    // Projectile_A (-15°)
    FRotator RotA = Owner->GetActorForwardVector().Rotation();
    RotA.Yaw -= 15.0f;
    World->SpawnActor<AProjectile_A>(AProjectile_A::StaticClass(), SpawnLocation, RotA, SpawnParams);

    // Projectile_B (recto)
    FRotator RotB = Owner->GetActorForwardVector().Rotation();
    World->SpawnActor<AProjectile_B>(AProjectile_B::StaticClass(), SpawnLocation, RotB, SpawnParams);

    // Projectile_C (+15°)
    FRotator RotC = Owner->GetActorForwardVector().Rotation();
    RotC.Yaw += 15.0f;
    World->SpawnActor<AProjectile_C>(AProjectile_C::StaticClass(), SpawnLocation, RotC, SpawnParams);

    if (FireSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, Owner->GetActorLocation());
    }

    //UE_LOG(LogTemp, Warning, TEXT("Spread pattern fired!"));
}

void UAtackComponent::FireBurst()
{
    // Dispara 3 veces rápido
    FireSingle();
    FireSingle();
    FireSingle();

    if (FireSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetOwner()->GetActorLocation());
    }
}

// ===== IMPLEMENTACIÓN DE NUEVOS ATAQUES PARA JEFES =====

void UAtackComponent::FireBossCircular()
{
    AActor* Owner = GetOwner();
    if (!Owner) return;

    UWorld* World = GetWorld();
    if (!World) return;

    FVector SpawnLocation = Owner->GetActorLocation() + FVector(0, 0, 0);
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = Owner;

    // Disparo circular en 8 direcciones (360°)
    int32 NumShots = 8;
    for (int32 i = 0; i < NumShots; i++)
    {
        float Angle = (360.0f / NumShots) * i;
        FRotator SpawnRotation = FRotator(0.0f, Angle, 0.0f);
        
        // Alterna entre los 3 tipos de proyectiles
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

    UE_LOG(LogTemp, Warning, TEXT("Boss Circular Attack - 360° barrage!"));
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

    // Disparo en espiral - 5 proyectiles en formación rotativa
    int32 NumShots = 5;
    for (int32 i = 0; i < NumShots; i++)
    {
        float AngleOffset = (360.0f / NumShots) * i;
        float FinalAngle = SpiralAngle + AngleOffset;
        FRotator SpawnRotation = FRotator(0.0f, FinalAngle, 0.0f);
        
        // Solo usa Projectile_A para espiral uniforme
        World->SpawnActor<AProjectile_A>(AProjectile_A::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
    }

    // Rotar para próximo disparo
    SpiralAngle += 25.0f; // Rotación gradual
    if (SpiralAngle >= 360.0f) SpiralAngle = 0.0f;

    if (FireSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, Owner->GetActorLocation());
    }

    UE_LOG(LogTemp, Warning, TEXT("Boss Spiral Attack - Rotating formation!"));
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

    // Ráfaga en ondas - disparo en arco frontal
    int32 NumWaves = 3;
    int32 ShotsPerWave = 5;
    
    for (int32 wave = 0; wave < NumWaves; wave++)
    {
        for (int32 shot = 0; shot < ShotsPerWave; shot++)
        {
            // Arco frontal de -60° a +60°
            float BaseAngle = -60.0f + (120.0f / (ShotsPerWave - 1)) * shot;
            float WaveOffset = wave * 10.0f; // Separación entre ondas
            float FinalAngle = BaseAngle + WaveOffset;
            
            FRotator SpawnRotation = FRotator(0.0f, FinalAngle, 0.0f);
            
            // Usa diferentes proyectiles por onda
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

    UE_LOG(LogTemp, Warning, TEXT("Boss Wave Barrage - Triple wave attack!"));
}

void UAtackComponent::FireBossTargeted()
{
    AActor* Owner = GetOwner();
    if (!Owner) return;

    UWorld* World = GetWorld();
    if (!World) return;

    // Buscar al jugador
    APawn* Player = UGameplayStatics::GetPlayerPawn(World, 0);
    if (!Player) return;

    FVector SpawnLocation = Owner->GetActorLocation();
    FVector PlayerLocation = Player->GetActorLocation();
    FVector Direction = (PlayerLocation - SpawnLocation).GetSafeNormal();
    
    FRotator TargetRotation = Direction.Rotation();
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = Owner;

    // Disparo dirigido principal
    World->SpawnActor<AProjectile_B>(AProjectile_B::StaticClass(), SpawnLocation, TargetRotation, SpawnParams);
    
    // Disparos adicionales con pequeña dispersión para hacer más difícil esquivar
    for (int32 i = 0; i < 4; i++)
    {
        float Spread = (i - 2) * 8.0f; // -16°, -8°, +8°, +16°
        FRotator SpreadRotation = TargetRotation;
        SpreadRotation.Yaw += Spread;
        
        World->SpawnActor<AProjectile_A>(AProjectile_A::StaticClass(), SpawnLocation, SpreadRotation, SpawnParams);
    }

    if (FireSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, Owner->GetActorLocation());
    }

    UE_LOG(LogTemp, Warning, TEXT("Boss Targeted Attack - Aimed at player!"));
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

    // ATAQUE ÉPICO FINAL - Combinación de todos los patrones
    
    // 1. Círculo completo (12 direcciones)
    for (int32 i = 0; i < 12; i++)
    {
        float Angle = (360.0f / 12) * i;
        FRotator SpawnRotation = FRotator(0.0f, Angle, 0.0f);
        World->SpawnActor<AProjectile_C>(AProjectile_C::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
    }

    // 2. Espiral doble
    for (int32 spiral = 0; spiral < 2; spiral++)
    {
        for (int32 i = 0; i < 8; i++)
        {
            float AngleOffset = (360.0f / 8) * i;
            float SpiralOffset = spiral * 180.0f; // Segunda espiral desfasada 180°
            float FinalAngle = SpiralAngle + AngleOffset + SpiralOffset;
            FRotator SpawnRotation = FRotator(0.0f, FinalAngle, 0.0f);
            
            World->SpawnActor<AProjectile_A>(AProjectile_A::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
        }
    }

    // 3. Ataque dirigido al jugador
    APawn* Player = UGameplayStatics::GetPlayerPawn(World, 0);
    if (Player)
    {
        FVector PlayerLocation = Player->GetActorLocation();
        FVector Direction = (PlayerLocation - SpawnLocation).GetSafeNormal();
        FRotator TargetRotation = Direction.Rotation();
        
        // Ráfaga dirigida
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

    UE_LOG(LogTemp, Warning, TEXT("BOSS EPIC FINAL ATTACK - ULTIMATE DEVASTATION!"));
}


