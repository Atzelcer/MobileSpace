// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Capsulas.h"
#include "Life.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESPACE_API ALife : public ACapsulas
{
	GENERATED_BODY()

public:

	ALife();
	UParticleSystemComponent* ParticleSystem;

};
