#include "Ship_CazadorRojo.h"
#include "UObject/ConstructorHelpers.h"

AShip_CazadorRojo::AShip_CazadorRojo()
{
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(
        TEXT("StaticMesh'/Game/StarSparrow/Meshes/Examples/SM_StarSparrow04.SM_StarSparrow04'")
    );

    if (MeshObj.Succeeded())
    {
        ShipMesh->SetStaticMesh(MeshObj.Object);
        ShipMesh->SetRelativeScale3D(FVector(0.3f));
    }

    Tipo = ENaveTipo::Roja;
    MovementPattern = EArcadeMovement::WavyWave;

    AttackPattern = EAttackPattern::Spread;
    ShipRole = EShipRole::Normal;

    TimeBetweenShots = 2.5f;
    bFireEnabled = true;
}

void AShip_CazadorRojo::BeginPlay()
{
    Super::BeginPlay();
    SetActorRotation(FRotator(0.f, -180.f, 0.f));
}

void AShip_CazadorRojo::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
