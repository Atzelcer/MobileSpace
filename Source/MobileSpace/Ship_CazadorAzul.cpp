#include "Ship_CazadorAzul.h"
#include "UObject/ConstructorHelpers.h"

AShip_CazadorAzul::AShip_CazadorAzul()
{
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(
        TEXT("StaticMesh'/Game/StarSparrow/Meshes/Examples/SM_StarSparrow02.SM_StarSparrow02'")
    );

    if (MeshObj.Succeeded())
    {
        ShipMesh->SetStaticMesh(MeshObj.Object);
        ShipMesh->SetRelativeScale3D(FVector(0.3f));
    }

    MovementPattern = EArcadeMovement::SuperSequence;

    AttackPattern = EAtackPattern::Single;

    TimeBetweenShots = 1.8f;
    bFireEnabled = true;
}

void AShip_CazadorAzul::BeginPlay()
{
    Super::BeginPlay();
    SetActorRotation(FRotator(0.f, -180.f, 0.f));
}

void AShip_CazadorAzul::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
