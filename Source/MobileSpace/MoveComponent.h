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
	SuperSequence
	
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
