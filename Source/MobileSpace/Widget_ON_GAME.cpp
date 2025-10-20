// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget_ON_GAME.h"
#include "Kismet/GameplayStatics.h"
#include "HUDmain.h"

void UWidget_ON_GAME::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_menu)
		Button_menu->OnClicked.AddDynamic(this, &UWidget_ON_GAME::OnMenuClicked);
}

void UWidget_ON_GAME::OnMenuClicked()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC) return;

	AHUDmain* HUD = Cast<AHUDmain>(PC->GetHUD());
	if (HUD)
	{
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		HUD->MostrarPause();
	}
}
