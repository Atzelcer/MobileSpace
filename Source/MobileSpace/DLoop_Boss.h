// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Boss_Z.h"
#include "DLoop_Boss.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESPACE_API ADLoop_Boss : public ABoss_Z
{
	GENERATED_BODY()

public:

	ADLoop_Boss();
	virtual void DispararAtaque() override;

	
};
