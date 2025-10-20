// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MegaPortal.generated.h"

class UCapsuleComponent;
class AAventuraManager;

UCLASS()
class MOBILESPACE_API AMegaPortal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMegaPortal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;


	UFUNCTION()
	void EstablecerMalla(UStaticMesh* NuevaMalla);

	UFUNCTION()
	void EscogerMallaAleatoria();

private:
	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshPortal;

	UPROPERTY()
	AAventuraManager* AventuraManagerRef;
};