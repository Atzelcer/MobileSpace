// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Boss_Z.h"
#include "DKraken_Boss_Z.generated.h"


UCLASS()
class MOBILESPACE_API ADKraken_Boss_Z : public ABoss_Z
{
	GENERATED_BODY()

public:
	ADKraken_Boss_Z();

	virtual void DispararAtaque() override;
};
