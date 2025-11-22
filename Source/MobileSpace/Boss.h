// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundBase.h"
#include "MoveComponent.h"
#include "AtackComponent.h"
#include "Boss.generated.h"

UCLASS()
class MOBILESPACE_API ABoss : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoss();

	// ===== COMPONENTES BASE =====
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* BossMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* BossCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UMoveComponent* MoveComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAtackComponent* AttackComp;

	// Componente de audio para sonidos del boss
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UAudioComponent* BossAudioComponent;

	// ===== PROPIEDADES DE COMBATE =====
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	int32 BossHealth = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	int32 CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float FireRate = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	EAtackPattern AttackPattern = EAtackPattern::Spread;

	// ===== EFECTOS Y AUDIO =====
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	UParticleSystem* DestructionEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	USoundBase* DestructionSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	FVector DestructionEffectScale = FVector(2.0f, 2.0f, 2.0f);

	// ===== ENTRADA ÉPICA =====
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Epic Entrance")
	USoundBase* AppearanceSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Epic Entrance")
	UParticleSystem* AppearanceEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Epic Entrance")
	float EntranceHeight = 800.0f; // Altura desde donde aparece

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Epic Entrance")
	float EntranceSpeed = 300.0f; // Velocidad de bajada

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Epic Entrance")
	float EntranceDuration = 3.0f; // Duración de la entrada

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Timer para disparo automático
	FTimerHandle FireTimerHandle;

	// Timer para ataque especial del jefe final
	FTimerHandle SpecialAttackTimerHandle;

	// ===== SISTEMA DE ENTRADA ÉPICA =====
	FTimerHandle EntranceTimerHandle;
	FVector FinalPosition; // Posición final donde quedará el jefe
	FVector StartPosition; // Posición inicial de entrada (arriba)
	bool bIsEntering = true; // ¿Está en animación de entrada?
	float EntranceTimeElapsed = 0.0f;
	
	// Control para evitar rotación inmediata después de entrada
	bool bCanRotate = false;

	// Función de disparo automático
	UFUNCTION()
	void AutoFire();

	// Función de ataque especial (solo para jefe final)
	UFUNCTION()
	virtual void SpecialAttack() {};

	// ===== FUNCIONES DE ENTRADA ÉPICA =====
	UFUNCTION()
	void StartEpicEntrance();

	UFUNCTION()
	void UpdateEntrance();

	UFUNCTION()
	void FinishEntrance();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ===== FUNCIONES DE COMBATE =====
	UFUNCTION()
	void OnBossHit(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void TakeDamage(int32 DamageAmount);

	UFUNCTION(BlueprintCallable)
	virtual void DestroyBoss();

};
