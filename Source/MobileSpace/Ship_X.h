// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ship_X.generated.h"

UCLASS()
class MOBILESPACE_API AShip_X : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShip_X();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* ShipMesh;

	// Movement properties for simple horizontal movement
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveSpeed = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")  
	float HorizontalSpeed = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Formation")
	FVector FormationPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Formation")
	bool bInFormation = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Formation")
	bool bAttacking = false;

	// Creative movement flags system
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Patterns")
	bool bMovingRight = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Patterns")
	bool bMovingDown = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Patterns")
	bool bZigzagMode = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Patterns")
	bool bPulseMode = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Patterns")
	bool bSerpentineMode = false;

	// Movement limits
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Limits")
	float LeftLimit = -800.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Limits")
	float RightLimit = 800.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Limits")
	float FrontLimit = 200.0f; // Y positive (toward player)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Limits")
	float BackLimit = 800.0f; // Y negative (away from player)

	// Creative movement parameters
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Patterns")
	float VerticalSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Patterns")
	float PulseIntensity = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Patterns")
	int32 ZigzagCounter = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Patterns")
	int32 ZigzagSteps = 3;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Timer for attack patterns
	FTimerHandle AttackTimerHandle;

	// Virtual functions for polymorphism
	virtual void UpdateMovement(float DeltaTime);
	virtual void StartAttackPattern();
	virtual void ReturnToFormation();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Public functions for formation control
	UFUNCTION(BlueprintCallable, Category = "Formation")
	void SetFormationPosition(FVector NewPosition);

	UFUNCTION(BlueprintCallable, Category = "Formation")
	void JoinFormation();

	UFUNCTION(BlueprintCallable, Category = "Formation")
	void StartAttack();

	// Creative movement pattern functions
	UFUNCTION(BlueprintCallable, Category = "Movement Patterns")
	void ActivateZigzagMode();

	UFUNCTION(BlueprintCallable, Category = "Movement Patterns") 
	void ActivatePulseMode();

	UFUNCTION(BlueprintCallable, Category = "Movement Patterns")
	void ActivateSerpentineMode();

	UFUNCTION(BlueprintCallable, Category = "Movement Patterns")
	void ActivateNormalMode();

};
