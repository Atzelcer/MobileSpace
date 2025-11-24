// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MoveComponent.generated.h"

UENUM(BlueprintType)
enum class EArcadeMovement : uint8
{
	None,
	Wave,
	SEntry,
	RadialBurst,
	WavyWave,
	ExpandingCircle,
	BossMajesticArc,
	SuperSequence,
	BossSlowSweep,          
	BossThreatening,        
	BossCircularDominance,  
	BossErraticPower,       
	BossEpicFinal,          
	// ===== PATRONES PARA ENJAMBRES =====
	SwarmEntry,             // Entrada dramática desde el lado derecho
	SwarmFormation,         // Movimiento en formación organizada
	SwarmAdvance            // Avance coordinado hacia el jugador
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MOBILESPACE_API UMoveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMoveComponent();

	UPROPERTY(EditAnywhere, Category = "ArcadeMove")
	EArcadeMovement Pattern = EArcadeMovement::WavyWave;

	UPROPERTY(EditAnywhere, Category = "ArcadeMove")
	float Speed = 520.0f;

	UPROPERTY(EditAnywhere, Category = "ArcadeMove")
	float Amplitude = 140.0f;

	UPROPERTY(EditAnywhere, Category = "ArcadeMove")
	float Frequency = 2.1f;

	UPROPERTY(EditAnywhere, Category = "ArcadeMove")
	FVector2D MovementMin = FVector2D(-1450.f, -2900.f);

	UPROPERTY(EditAnywhere, Category = "ArcadeMove")
	FVector2D MovementMax = FVector2D(1450.f, 2900.f);

	// ===== VARIABLES PARA ENJAMBRES =====
	UPROPERTY(EditAnywhere, Category = "Swarm")
	FVector SwarmTargetPosition = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Swarm")
	float SwarmEntrySpeed = 800.0f;

	UPROPERTY(EditAnywhere, Category = "Swarm")
	float SwarmFormationSpeed = 300.0f;

	UPROPERTY(EditAnywhere, Category = "Swarm")
	float SwarmAdvanceSpeed = 150.0f;

	// Estado de la fase del enjambre
	UPROPERTY(BlueprintReadOnly, Category = "Swarm")
	int32 SwarmPhase = 0; // 0=Entry, 1=Formation, 2=Advance

	UFUNCTION(BlueprintCallable, Category = "Swarm")
	void SetSwarmTargetPosition(FVector NewTarget);

	UFUNCTION(BlueprintCallable, Category = "Swarm")
	void SetSwarmPhase(int32 NewPhase);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	FVector Origin;
	float Elapsed;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
