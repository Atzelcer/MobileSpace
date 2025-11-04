// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetGameOver.h"
#include "Kismet/GameplayStatics.h"
#include "HUDmain.h"
#include "Engine/Engine.h"

void UWidgetGameOver::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_volvera_empezar)
		Button_volvera_empezar->OnClicked.AddDynamic(this, &UWidgetGameOver::OnVolverEmpezarClicked);

	if (Button_salir_panel_principal)
		Button_salir_panel_principal->OnClicked.AddDynamic(this, &UWidgetGameOver::OnSalirPanelPrincipalClicked);

}

void UWidgetGameOver::OnVolverEmpezarClicked()
{
	//Eliminar todo y entrar a la pantalla de carga 
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC) return;
	AHUDmain* HUD = Cast<AHUDmain>(PC->GetHUD());
	if (HUD)
	{
		HUD->OcultarOnGame();
		HUD->OcultarGameOver();
		FName NivelActual = *UGameplayStatics::GetCurrentLevelName(GetWorld());
		UGameplayStatics::OpenLevel(GetWorld(), NivelActual);
		UGameplayStatics::SetGamePaused(GetWorld(), false);
		HUD->OcultarPanelPrincipal();
		HUD->MostrarPantallaCarga();
	}
}

void UWidgetGameOver::OnSalirPanelPrincipalClicked()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC) return;

	AHUDmain* HUD = Cast<AHUDmain>(PC->GetHUD());
	if (HUD)
	{
		HUD->OcultarOnGame();
		FName NivelActual = *UGameplayStatics::GetCurrentLevelName(GetWorld());
		UGameplayStatics::OpenLevel(GetWorld(), NivelActual);
		UGameplayStatics::SetGamePaused(GetWorld(), false);
		HUD->MostrarPanelPrincipal();
	}
}
