// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MoveComponent.generated.h"

UENUM(BlueprintType)
enum class EMovementPattern : uint8
{
	Linear          UMETA(DisplayName = "Linear Movement"),     
	SineWave        UMETA(DisplayName = "Sine Wave"),           
	Circular        UMETA(DisplayName = "Circular Motion"),     
	ZigZag          UMETA(DisplayName = "Zig Zag"),            
	Elliptical      UMETA(DisplayName = "Elliptical Path")     
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

	// LÍMITES DE PANTALLA PARA MÓVIL
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mobile Bounds")
	float MinX = -1600.0f; // Límite izquierdo (donde está el jugador)
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mobile Bounds")
	float MaxX = 1600.0f; // Límite derecho (donde spawnan)
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mobile Bounds")
	float MinY = -800.0f; // Límite inferior
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mobile Bounds")
	float MaxY = 800.0f; // Límite superior

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mobile Bounds")
	bool bUseBounds = true; // Activar/desactivar límites


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	EMovementPattern CurrentPattern;
	float MovementTime;
	int32 PatrolDirection;


	// Funciones de patrones matemáticos simples (solo X, Y)
	void UpdateLinear(float DeltaTime);
	void UpdateSineWave(float DeltaTime);
	void UpdateCircular(float DeltaTime);
	void UpdateZigZag(float DeltaTime);
	void UpdateElliptical(float DeltaTime);

	// Función para aplicar límites de pantalla
	FVector ApplyScreenBounds(const FVector& DesiredPosition);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
