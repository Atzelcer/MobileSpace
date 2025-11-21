#include "Ship_CazadorAlfa.h"
#include "UObject/ConstructorHelpers.h"

AShip_CazadorAlfa::AShip_CazadorAlfa()
{
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(
        TEXT("StaticMesh'/Game/StarSparrow/Meshes/Examples/SM_StarSparrow16.SM_StarSparrow16'")
    );

    if (MeshObj.Succeeded())
    {
        ShipMesh->SetStaticMesh(MeshObj.Object);
        ShipMesh->SetRelativeScale3D(FVector(0.3f));
    }

    Tipo = ENaveTipo::Alfa;
    MovementPattern = EArcadeMovement::SEntry;

    AttackPattern = EAttackPattern::Burst;     // corregido
    ShipRole = EShipRole::Perseguidor; 

    TimeBetweenShots = 1.5f;
    bFireEnabled = true;
}

void AShip_CazadorAlfa::BeginPlay()
{
    Super::BeginPlay();
    SetActorRotation(FRotator(0.f, -180.f, 0.f));
}

void AShip_CazadorAlfa::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
