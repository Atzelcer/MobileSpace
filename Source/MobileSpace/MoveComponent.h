// Fill out your copyright notice...

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MobEnums.h"
#include "MoveComponent.generated.h"

//UENUM(BlueprintType)
//enum class EArcadeMovement : uint8
//{
//    None,
//    Wave,
//    SEntry,
//    RadialBurst,
//    WavyWave,
//    ExpandingCircle,
//    BossMajesticArc,
//    SuperSequence
//};

UENUM(BlueprintType)
enum class ERotationMode : uint8
{
    Exact,
    Smooth
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MOBILESPACE_API UMoveComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UMoveComponent();

    UPROPERTY(EditAnywhere)
    bool bMovementEnabled = true;

    UPROPERTY(EditAnywhere)
    EArcadeMovement Pattern = EArcadeMovement::WavyWave;

    UPROPERTY(EditAnywhere)
    ERotationMode RotationMode = ERotationMode::Smooth;

    UPROPERTY(EditAnywhere)
    float OrientationSpeed = 8.0f;

    UPROPERTY(EditAnywhere)
    float Speed = 640.0f;

    UPROPERTY(EditAnywhere)
    float FollowSpeed = 880.0f;

    UPROPERTY(EditAnywhere)
    float FollowIntensity = 1.35f;

    UPROPERTY(EditAnywhere)
    float Amplitude = 140.0f;

    UPROPERTY(EditAnywhere)
    float Frequency = 2.1f;

    UPROPERTY(EditAnywhere)
    FVector ConstantDirection = FVector::ZeroVector;

    UPROPERTY(EditAnywhere)
    float ConstantSpeed = 900.0f;

    UPROPERTY(EditAnywhere)
    AActor* FollowTarget = nullptr;

    UPROPERTY(EditAnywhere)
    FVector2D MovementMin = FVector2D(-1450.f, -2900.f);

    UPROPERTY(EditAnywhere)
    FVector2D MovementMax = FVector2D(1450.f, 2900.f);

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    void EnableMovement(bool bEnable);
    void SetPattern(EArcadeMovement NewPattern);
    void SetRotationMode(ERotationMode Mode);
    void SetFollowTarget(AActor* Target);
    void SetConstantDirection(FVector Dir);
    void ResetOrigin();

private:
    FVector Origin;
    float Elapsed;

    FVector ComputePatternMovement(float DeltaTime);
    FVector ComputeFollowMovement(float DeltaTime);
    FVector ComputeConstantMovement(float DeltaTime);

    void ApplyRotation(const FVector& MoveDirection, float DeltaTime);
    FVector ClampPosition(const FVector& Pos) const;
};
