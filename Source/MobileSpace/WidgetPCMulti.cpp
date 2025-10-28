// Fill out your copyright notice in the Description page of Project Settings.

#include "WidgetPCMulti.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "HUDmain.h"

void UWidgetPCMulti::NativeConstruct()
{
	Super::NativeConstruct();

	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
		{
			IrAlJuego();
		});
}

void UWidgetPCMulti::IrAlJuego()
{
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
		{
			AHUDmain* HUD = Cast<AHUDmain>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
			if (HUD)
				HUD->MostrarOnGameMulti();

		}, 3.0f, false);
}
