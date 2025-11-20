// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AtackComponent.generated.h"
UENUM(BlueprintType)
enum class EAtackPattern : uint8
{
	Single      UMETA(DisplayName = "Single Shot"),
	Spread      UMETA(DisplayName = "Spread"),
	Burst       UMETA(DisplayName = "Burst"),
	// ===== NUEVOS PATRONES PARA JEFES =====
	BossCircular        UMETA(DisplayName = "Boss Circular - 360 degrees"),
	BossSpiral          UMETA(DisplayName = "Boss Spiral - Rotating spread"),
	BossWaveBarrage     UMETA(DisplayName = "Boss Wave Barrage"),
	BossTargeted        UMETA(DisplayName = "Boss Targeted - Aims at player"),
	BossEpicFinal       UMETA(DisplayName = "Boss Epic Final - Ultimate attack")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MOBILESPACE_API UAtackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAtackComponent();

	UFUNCTION(BlueprintCallable)
	void Fire(EAtackPattern Pattern);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float FireRate = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	USoundBase* FireSound;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:
	bool bCanFire;
	FTimerHandle FireTimerHandle;

	void OnFireTimerExpired();

	void FireSingle();
	void FireSpread();
	void FireBurst();
	
	// ===== NUEVOS ATAQUES PARA JEFES =====
	void FireBossCircular();        // Disparo circular 360 grados
	void FireBossSpiral();          // Disparo en espiral rotativa
	void FireBossWaveBarrage();     // Ráfaga en ondas
	void FireBossTargeted();        // Disparo dirigido al jugador
	void FireBossEpicFinal();       // Ataque épico final

	// Para patrones rotativos
	float SpiralAngle = 0.0f;

	// Para alternar proyectiles en single
	int32 ProjectileCycle = 0;
		
};
