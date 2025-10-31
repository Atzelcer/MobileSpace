// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Capsulas.h"
#include "LevelUP.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESPACE_API ALevelUP : public ACapsulas
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALevelUP();

	UParticleSystemComponent* LevelUpEffect;
	
};
