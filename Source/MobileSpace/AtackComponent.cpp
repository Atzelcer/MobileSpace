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


