#include "Ship_CazadorOmega.h"
#include "UObject/ConstructorHelpers.h"

AShip_CazadorOmega::AShip_CazadorOmega()
{
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(
        TEXT("StaticMesh'/Game/StarSparrow/Meshes/Examples/SM_StarSparrow15.SM_StarSparrow15'")
    );

    if (MeshObj.Succeeded())
    {
        ShipMesh->SetStaticMesh(MeshObj.Object);
        ShipMesh->SetRelativeScale3D(FVector(0.5f));
    }

    Tipo = ENaveTipo::Omega;
    MovementPattern = EArcadeMovement::ExpandingCircle;

    AttackPattern = EAttackPattern::Spread;
    ShipRole = EShipRole::Tanque;

    TimeBetweenShots = 1.0f;
    bFireEnabled = true;
}

void AShip_CazadorOmega::BeginPlay()
{
    Super::BeginPlay();
    SetActorRotation(FRotator(0.f, -180.f, 0.f));
}

void AShip_CazadorOmega::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
