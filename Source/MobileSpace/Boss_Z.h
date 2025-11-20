#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystem.h"
#include "MoveComponent.h"
#include "Boss_Z.generated.h"

class AHUDmain;

UCLASS()
class MOBILESPACE_API ABoss_Z : public ACharacter
{
	GENERATED_BODY()

public:
	ABoss_Z();

protected:
	virtual void BeginPlay() override;

	// NOTIFY ? Evento general de overlap del actor
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	// EVENTO DE OVERLAP DEL COMPONENTE
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
	virtual void Tick(float DeltaTime) override;

	// Componentes
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BossMesh;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* ShipCollision;

	UPROPERTY(VisibleAnywhere)
	UMoveComponent* MoveComp;

	// Vida
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxHealth = 100.f;

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	float CurrentHealth;

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	bool bIsDead = false;

	// Efectos
	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* SpawnParticle;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* DeathParticle;

	// Funciones
	void SpawnSequence();
	void DeathSequence();
	virtual void DispararAtaque();

	// Referencia al HUD
	UPROPERTY()
	AHUDmain* HUD;
};
