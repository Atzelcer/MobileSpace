// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetMegaBoss.h"

void UWidgetMegaBoss::NativeConstruct()
{
	Super::NativeConstruct();
}

void UWidgetMegaBoss::UpdateBossLife(float Current, float Max)
{
	if (!ProgressBar_Bosss || !TextBlock_porcentajjeVida || Max <= 0.f) return;

	float Percent = Current / Max;

	ProgressBar_Bosss->SetPercent(Percent);

	int32 PorcentajeEntero = FMath::RoundToInt(Percent * 100.f);
	FString Texto = FString::Printf(TEXT("%d%%"), PorcentajeEntero);
	TextBlock_porcentajjeVida->SetText(FText::FromString(Texto));
}
