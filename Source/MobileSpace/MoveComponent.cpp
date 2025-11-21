#include "MoveComponent.h"
#include "GameFramework/Actor.h"

UMoveComponent::UMoveComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    Elapsed = 0.f;
}

void UMoveComponent::BeginPlay()
{
    Super::BeginPlay();
    if (GetOwner())
        Origin = GetOwner()->GetActorLocation();
}

void UMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if (!bMovementEnabled || !GetOwner()) return;

    Elapsed += DeltaTime;
    FVector MoveDir = FVector::ZeroVector;

    if (FollowTarget)
        MoveDir += ComputeFollowMovement(DeltaTime);

    if (!ConstantDirection.IsZero())
        MoveDir += ComputeConstantMovement(DeltaTime);

    if (Pattern != EArcadeMovement::None)
        MoveDir += ComputePatternMovement(DeltaTime);

    if (!MoveDir.IsZero())
    {
        ApplyRotation(MoveDir, DeltaTime);
        FVector NewPos = GetOwner()->GetActorLocation() + MoveDir * DeltaTime;
        NewPos = ClampPosition(NewPos);
        GetOwner()->SetActorLocation(NewPos);
    }
}

void UMoveComponent::EnableMovement(bool bEnable)
{
    bMovementEnabled = bEnable;
}

void UMoveComponent::SetPattern(EArcadeMovement NewPattern)
{
    Pattern = NewPattern;
    Elapsed = 0.f;
    ResetOrigin();
}

void UMoveComponent::SetRotationMode(ERotationMode Mode)
{
    RotationMode = Mode;
}

void UMoveComponent::SetFollowTarget(AActor* Target)
{
    FollowTarget = Target;
}

void UMoveComponent::SetConstantDirection(FVector Dir)
{
    ConstantDirection = Dir;
}

void UMoveComponent::ResetOrigin()
{
    if (GetOwner())
        Origin = GetOwner()->GetActorLocation();
    Elapsed = 0.f;
}

FVector UMoveComponent::ComputeConstantMovement(float DeltaTime)
{
    return ConstantDirection.GetSafeNormal() * ConstantSpeed;
}

FVector UMoveComponent::ComputeFollowMovement(float DeltaTime)
{
    if (!FollowTarget || !GetOwner()) return FVector::ZeroVector;

    FVector Dir = (FollowTarget->GetActorLocation() - GetOwner()->GetActorLocation()).GetSafeNormal();
    return Dir * FollowSpeed * FollowIntensity;
}

FVector UMoveComponent::ComputePatternMovement(float DeltaTime)
{
    FVector Result = FVector::ZeroVector;
    float t = Elapsed;

    float X = Origin.X;
    float Y = Origin.Y;

    switch (Pattern)
    {
    case EArcadeMovement::Wave:
        X -= Speed * t;
        Y += Amplitude * FMath::Sin(Frequency * t);
        break;

    case EArcadeMovement::WavyWave:
    {
        float T1 = 2.1f;
        float T2 = 0.4f;
        float T3 = 0.7f;
        FVector2D Center = (MovementMin + MovementMax) * 0.5f;

        float Phase = t;

        if (Phase < T1)
        {
            float u = 2.f * PI * Phase / T1;
            X = Center.X + 110.f * FMath::Sin(u) * (FMath::Exp(FMath::Cos(u)) - 2 * FMath::Cos(4 * u));
            Y = Center.Y + 90.f * FMath::Cos(u);
        }
        else if (Phase < T1 + T2)
        {
            float k = (Phase - T1) / T2;
            float DX = MovementMin.X + FMath::Fmod(GetOwner()->GetUniqueID() * 147, int(MovementMax.X - MovementMin.X));
            float DY = MovementMin.Y + FMath::Fmod(GetOwner()->GetUniqueID() * 71, int(MovementMax.Y - MovementMin.Y));
            X = FMath::Lerp(X, DX, k);
            Y = FMath::Lerp(Y, DY, k);
        }
        else if (Phase < T1 + T2 + T3)
        {
            float k = (Phase - T1 - T2) / T3;
            X = FMath::Lerp(X, Center.X, k) + 40.f * FMath::Cos(k * 4.5f);
            Y = FMath::Lerp(Y, Center.Y, k) + 35.f * FMath::Sin(k * 3.2f);
        }
        else
        {
            Elapsed = 0.f;
        }
    }
    break;

    default:
        X -= Speed * t;
        break;
    }

    FVector Current = GetOwner()->GetActorLocation();
    Result.X = X - Current.X;
    Result.Y = Y - Current.Y;
    return Result;
}

void UMoveComponent::ApplyRotation(const FVector& MoveDirection, float DeltaTime)
{
    if (!GetOwner()) return;

    FRotator TargetRot = MoveDirection.GetSafeNormal().Rotation();

    if (RotationMode == ERotationMode::Exact)
    {
        GetOwner()->SetActorRotation(TargetRot);
    }
    else
    {
        FRotator Current = GetOwner()->GetActorRotation();
        FRotator Smooth = FMath::RInterpTo(Current, TargetRot, DeltaTime, OrientationSpeed);
        GetOwner()->SetActorRotation(Smooth);
    }
}

FVector UMoveComponent::ClampPosition(const FVector& Pos) const
{
    FVector R = Pos;
    R.X = FMath::Clamp(R.X, MovementMin.X, MovementMax.X);
    R.Y = FMath::Clamp(R.Y, MovementMin.Y, MovementMax.Y);
    return R;
}
