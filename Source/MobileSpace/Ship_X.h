#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystem.h"
#include "AtackComponent.h"
#include "MoveComponent.h"
#include "Sound/SoundBase.h"
#include "Ship_X.generated.h"


UCLASS()
class MOBILESPACE_API AShip_X : public AActor
{
    GENERATED_BODY()

public:

    AShip_X();

 

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ship")
    EArcadeMovement MovementPattern;


    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* ShipMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* ShipCollision;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UMoveComponent* MoveComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UAtackComponent* AttackComp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
    USoundBase* DestructionSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
    UParticleSystem* DestructionEffect;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VFX")
    UParticleSystemComponent* TrailEffect;


    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector DestructionEffectScale;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Behavior")
    bool bFireEnabled;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Behavior")
    bool bUseFormation;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Behavior")
    FVector FormationAnchor;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Behavior")
    FVector FormationOffset;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    float TimeBetweenShots = 2.0f;

	EAtackPattern AttackPattern;


protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    void SetMovement(EArcadeMovement NewPattern);
    void SetAttackPattern(EAtackPattern NewPattern);
    void EnableFire(bool Enabled);

    UFUNCTION()
    void OnShipHit(UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);

    UFUNCTION()
    void HandleDestruction();

protected:
    UPROPERTY(EditAnywhere, Category = "PowerUps")
    int32 ProbabilidadSpawnCapsula;

    UPROPERTY(EditDefaultsOnly, Category = "Audio")
    USoundBase* SonidoExplosion;

    void IntentarSpawnCapsula();

public:
    virtual void DestruirNave();

    bool GetAttackState() const;
    void SetAttackState(bool Estado);

    // Sistema de disparo automático
    UFUNCTION()
    void StartAutoFire();
    
protected:
    // Timer para disparo automático
    FTimerHandle AutoFireTimer;
    
    // Función de disparo automático
    UFUNCTION()
    void AutoFire();

public:
    bool bPuedeAtacar = false;


};
