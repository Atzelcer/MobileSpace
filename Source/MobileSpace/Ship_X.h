// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "MoveComponent.h"
#include "Ship_X.generated.h"


UCLASS()
class MOBILESPACE_API AShip_X : public AActor
{
	GENERATED_BODY()
	
public:	
	AShip_X();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* ShipMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* ShipCollision;
	UPROPERTY(VisibleAnywhere)
	UMoveComponent* MoveComp;
	

protected:
	virtual void BeginPlay() override;



public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnShipHit(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	

};
