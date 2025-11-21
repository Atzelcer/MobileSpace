#include "Ship_CazadorDelta.h"
#include "UObject/ConstructorHelpers.h"

AShip_CazadorDelta::AShip_CazadorDelta()
{
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(
        TEXT("StaticMesh'/Game/StarSparrow/Meshes/Examples/SM_StarSparrow19.SM_StarSparrow19'")
    );

    if (MeshObj.Succeeded())
    {
        ShipMesh->SetStaticMesh(MeshObj.Object);
        ShipMesh->SetRelativeScale3D(FVector(0.3f));
    }

    Tipo = ENaveTipo::Delta;
    MovementPattern = EArcadeMovement::SuperSequence;

    AttackPattern = EAttackPattern::Burst;
    ShipRole = EShipRole::Rafaga;

    TimeBetweenShots = 2.5f;
    bFireEnabled = true;
}

void AShip_CazadorDelta::BeginPlay()
{
    Super::BeginPlay();
    SetActorRotation(FRotator(0.f, -180.f, 0.f));
}

void AShip_CazadorDelta::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
