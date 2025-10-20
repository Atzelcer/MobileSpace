// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget_pause.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "HUDmain.h"

void UWidget_pause::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_CONTINUAR)
		Button_CONTINUAR->OnClicked.AddDynamic(this, &UWidget_pause::OnContinuarClicked);

	if (Button_ajustes)
		Button_ajustes->OnClicked.AddDynamic(this, &UWidget_pause::OnAjustesClicked);

	if (Button_MENU_PRINCIPAL)
		Button_MENU_PRINCIPAL->OnClicked.AddDynamic(this, &UWidget_pause::OnMenuPrincipalClicked);
}

void UWidget_pause::OnContinuarClicked()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	RemoveFromParent();
}

void UWidget_pause::OnAjustesClicked()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC) return;

	AHUDmain* HUD = Cast<AHUDmain>(PC->GetHUD());
	if (HUD)
	{
		HUD->MostrarAjustes();
	}
}

void UWidget_pause::OnMenuPrincipalClicked()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC) return;

	AHUDmain* HUD = Cast<AHUDmain>(PC->GetHUD());
	if (HUD)
	{
		HUD->OcultarOnGame();
		HUD->OcultarPause();
		HUD->OcultarTodo();
		FName NivelActual = *UGameplayStatics::GetCurrentLevelName(GetWorld());
		UGameplayStatics::OpenLevel(GetWorld(), NivelActual);
		UGameplayStatics::SetGamePaused(GetWorld(), false);
		HUD->MostrarPanelPrincipal();
	}
}
