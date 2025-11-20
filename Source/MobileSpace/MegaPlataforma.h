// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MegaPlataforma.generated.h"

UCLASS()
class MOBILESPACE_API AMegaPlataforma : public AActor
{
	GENERATED_BODY()

public:
	AMegaPlataforma();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void CambiarMaterialAleatorio();
	void CambiarMaterialPorIndice(int32 Index);

private:
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* PlataformaMesh;

	UPROPERTY()
	TArray<UMaterialInterface*> Materiales;
};
