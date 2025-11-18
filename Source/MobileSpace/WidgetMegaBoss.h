// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "WidgetMegaBoss.generated.h"

UCLASS()
class MOBILESPACE_API UWidgetMegaBoss : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintCallable)
	void UpdateBossLife(float Current, float Max);

protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ProgressBar_Bosss;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock_porcentajjeVida;
};
