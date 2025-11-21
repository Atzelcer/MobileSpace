#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MobEnums.h"
#include "AtackComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MOBILESPACE_API UAtackComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UAtackComponent();

    UPROPERTY(EditAnywhere)
    bool bAttackEnabled = true;

    UPROPERTY(EditAnywhere)
    EAttackPattern Pattern = EAttackPattern::Single;

    UPROPERTY(EditAnywhere)
    float FireRate = 0.3f;

    UPROPERTY(EditAnywhere)
    float SpreadAngle = 15.0f;

    UPROPERTY(EditAnywhere)
    float BurstDelay = 0.08f;

    UPROPERTY(EditAnywhere)
    TSubclassOf<AActor> ProjectileA;

    UPROPERTY(EditAnywhere)
    TSubclassOf<AActor> ProjectileB;

    UPROPERTY(EditAnywhere)
    TSubclassOf<AActor> ProjectileC;

    UPROPERTY(EditAnywhere)
    USoundBase* FireSound;

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    void RequestFire();
    void EnableAttack(bool bEnable);
    void SetPattern(EAttackPattern NewPattern);
    void SetProjectileClasses(TSubclassOf<AActor> A, TSubclassOf<AActor> B, TSubclassOf<AActor> C);

private:
    bool bCanFire;
    FTimerHandle TimerHandle_FireRate;
    int32 BurstCount;
    int32 ProjectileCycle;

    void ResetFire();
    void ExecuteFirePattern();
    void FireSingle();
    void FireSpread();
    void FireBurst();
    void FireTripleArc();
    void FireSniperShot();
    void FireCircular();
    void SpawnProjectile(TSubclassOf<AActor> ProjClass, const FVector& Loc, const FRotator& Rot);
};
