// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h" 
#include "Sound/SoundBase.h"
#include "MoveComponent.h"
#include "AtackComponent.h"
#include "Boss.generated.h"

class AHUDmain;
class UWidgetMegaBoss;

UCLASS()
class MOBILESPACE_API ABoss : public AActor
{
	GENERATED_BODY()
	
public:	
	ABoss();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* BossMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* BossCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UMoveComponent* MoveComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAtackComponent* AttackComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UAudioComponent* BossAudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	int32 BossHealth = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	int32 CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float FireRate = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	EAtackPattern AttackPattern = EAtackPattern::Spread;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	UParticleSystem* DestructionEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UParticleSystemComponent* TrailParticleComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	USoundBase* DestructionSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	FVector DestructionEffectScale = FVector(2.0f, 2.0f, 2.0f);



	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Epic Entrance")
	USoundBase* AppearanceSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Epic Entrance")
	UParticleSystem* AppearanceEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Epic Entrance")
	float EntranceHeight = 800.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Epic Entrance")
	float EntranceSpeed = 300.0f; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Epic Entrance")
	float EntranceDuration = 3.0f; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX|Trail")
	bool bTrailActiveOnSpawn = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX|Trail")
	FVector TrailOffset = FVector(0.f, 0.f, 0.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX|Trail")
	UParticleSystem* TrailEffect = nullptr;

protected:
	
	virtual void BeginPlay() override;

	
	FTimerHandle FireTimerHandle;

	
	FTimerHandle SpecialAttackTimerHandle;

	FTimerHandle EntranceTimerHandle;
	FVector FinalPosition; 
	FVector StartPosition; 
	bool bIsEntering = true; 
	float EntranceTimeElapsed = 0.0f;
	
	bool bCanRotate = false;

	

	UPROPERTY()
	AHUDmain* MainHUD;

	UFUNCTION()
	void AutoFire();

	UFUNCTION()
	virtual void SpecialAttack() {};

	UFUNCTION()
	void StartEpicEntrance();

	UFUNCTION()
	void UpdateEntrance();

	UFUNCTION()
	void FinishEntrance();

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnBossHit(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void ApplyBossDamage(int32 DamageAmount);


	UFUNCTION(BlueprintCallable)
	virtual void DestroyBoss();

	UFUNCTION(BlueprintCallable)
	void ShowBossHealthBar();

	UFUNCTION(BlueprintCallable)
	void HideBossHealthBar();

	UFUNCTION(BlueprintCallable)
	void UpdateBossHealthBar();

	UFUNCTION(BlueprintCallable, Category = "VFX")
	void ActivateTrail();

	UFUNCTION(BlueprintCallable, Category = "VFX")
	void DeactivateTrail();



};
