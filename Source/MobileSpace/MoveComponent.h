// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MoveComponent.generated.h"

UENUM(BlueprintType)
enum class EMovementPattern : uint8
{
	Idle            UMETA(DisplayName = "Idle"),
	Patrol          UMETA(DisplayName = "Horizontal Patrol"),
	SineWave        UMETA(DisplayName = "Sine Wave"),
	Figure8         UMETA(DisplayName = "Figure 8"),
	Spiral          UMETA(DisplayName = "Spiral"),
	Loop            UMETA(DisplayName = "Loop"),
	ZigZag          UMETA(DisplayName = "Zig Zag"),
	Parabola        UMETA(DisplayName = "Parabolic Dive")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MOBILESPACE_API UMoveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMoveComponent();
	// Control de patrones
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void StartPattern(EMovementPattern Pattern);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void StopMovemento();

	// Parámetros editables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Speed = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Amplitude = 200.0f; // Amplitud de ondas/espirales

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Frequency = 2.0f; // Frecuencia de oscilación

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector StartPosition = FVector::ZeroVector;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	EMovementPattern CurrentPattern;
	float MovementTime;
	int32 PatrolDirection;


	// Funciones de patrones
	void UpdatePatrol(float DeltaTime);
	void UpdateSineWave(float DeltaTime);
	void UpdateFigure8(float DeltaTime);
	void UpdateSpiral(float DeltaTime);
	void UpdateLoop(float DeltaTime);
	void UpdateZigZag(float DeltaTime);
	void UpdateParabola(float DeltaTime);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
