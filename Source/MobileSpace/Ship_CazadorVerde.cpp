#include "Ship_CazadorVerde.h"
#include "MobEnums.h"
#include "UObject/ConstructorHelpers.h"

AShip_CazadorVerde::AShip_CazadorVerde()
{
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(
        TEXT("StaticMesh'/Game/StarSparrow/Meshes/Examples/SM_StarSparrow05.SM_StarSparrow05'")
    );

    if (MeshObj.Succeeded())
    {
        ShipMesh->SetStaticMesh(MeshObj.Object);
        ShipMesh->SetRelativeScale3D(FVector(0.4f));
    }

    Tipo = ENaveTipo::Verde;
    MovementPattern = EArcadeMovement::SuperSequence;

    AttackPattern = EAttackPattern::Single;
    ShipRole = EShipRole::Normal;

    TimeBetweenShots = 2.0f;
    bFireEnabled = true;
}

void AShip_CazadorVerde::BeginPlay()
{
    Super::BeginPlay();
    SetActorRotation(FRotator(0.f, -180.f, 0.f));
}

void AShip_CazadorVerde::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
