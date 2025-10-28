// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "Animation/AnimSequence.h"
#include "Boss_Z.generated.h"

UENUM(BlueprintType)
enum class EBossState : uint8
{
	Idle,
	Attack,
	Death
};

UCLASS()
class MOBILESPACE_API ABoss_Z : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABoss_Z();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Animation sequences - to be set by child classes
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	UAnimSequence* IdleAnimation;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	UAnimSequence* AttackAnimation;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	UAnimSequence* DeathAnimation;

	// Boss state management
	UPROPERTY(BlueprintReadOnly, Category = "Boss State")
	EBossState CurrentState;

	// Timer handles
	UPROPERTY()
	FTimerHandle IdleTimerHandle;

	UPROPERTY()
	FTimerHandle DeathTimerHandle;

	

	
	// Animation state methods
	virtual void StartIdlePhase();
	virtual void StartAttackPhase();
	virtual void StartDeathPhase();
	virtual void DestroyBoss();

	

	// Collision hit event
	UFUNCTION()
	virtual void OnBossHit(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	USkeletalMeshComponent* BossMesh;

	UBoxComponent* ShipCollision;

private:
	// Time constants
	static constexpr float IDLE_DURATION = 5.0f;
};

