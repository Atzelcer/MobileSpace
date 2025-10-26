// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget_Modo_multijugador.h"
#include "Kismet/GameplayStatics.h"
#include "HUDmain.h"
#include "WidgetSalaEspera.h"
#include "Engine/Engine.h"

void UWidget_Modo_multijugador::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_crear_sala)
		Button_crear_sala->OnClicked.AddDynamic(this, &UWidget_Modo_multijugador::OnCrearSalaClicked);

	if (Button_entrar_sala)
		Button_entrar_sala->OnClicked.AddDynamic(this, &UWidget_Modo_multijugador::OnEntrarSalaClicked);

	if (Button_volver)
		Button_volver->OnClicked.AddDynamic(this, &UWidget_Modo_multijugador::OnVolverClicked);

	if (Button_cerrar_ventana_sala)
		Button_cerrar_ventana_sala->OnClicked.AddDynamic(this, &UWidget_Modo_multijugador::OnCerrarVentanaClicked);

	if (Button_entrar_sala_ventana)
		Button_entrar_sala_ventana->OnClicked.AddDynamic(this, &UWidget_Modo_multijugador::OnEntrarSalaVentanaClicked);

	// Ocultar ventana secundaria al iniciar
	MostrarVentana(false);
}

void UWidget_Modo_multijugador::MostrarVentana(bool bMostrar)
{
	ESlateVisibility Estado = bMostrar ? ESlateVisibility::Visible : ESlateVisibility::Collapsed;

	if (Image_fondo_Ventana) Image_fondo_Ventana->SetVisibility(Estado);
	if (Image_marco_entrar) Image_marco_entrar->SetVisibility(Estado);
	if (TextBlock_Ventana_sala) TextBlock_Ventana_sala->SetVisibility(Estado);
	if (codigoSala_NEW) codigoSala_NEW->SetVisibility(Estado);
	if (Button_entrar_sala_ventana) Button_entrar_sala_ventana->SetVisibility(Estado);
	if (Button_cerrar_ventana_sala) Button_cerrar_ventana_sala->SetVisibility(Estado);
}

void UWidget_Modo_multijugador::OnCrearSalaClicked()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC) return;

	AHUDmain* HUD = Cast<AHUDmain>(PC->GetHUD());
	if (!HUD) return;

	HUD->MostrarSalaEspera();
	HUD->OcultarModoMultijugador();
	HUD->OcultarTodo();
}

void UWidget_Modo_multijugador::OnEntrarSalaClicked()
{
	MostrarVentana(true);
}

void UWidget_Modo_multijugador::OnCerrarVentanaClicked()
{
	MostrarVentana(false);
}

void UWidget_Modo_multijugador::OnEntrarSalaVentanaClicked()
{
	if (!codigoSala_NEW) return;

	const FString CodigoIngresado = codigoSala_NEW->GetText().ToString();
	if (CodigoIngresado.Len() <= 0) return;

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC) return;

	AHUDmain* HUD = Cast<AHUDmain>(PC->GetHUD());
	if (HUD)
	{
		HUD->MostrarSalaEspera();
		HUD->OcultarModoMultijugador();
	}
}

void UWidget_Modo_multijugador::OnVolverClicked()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC) return;

	AHUDmain* HUD = Cast<AHUDmain>(PC->GetHUD());
	if (HUD)
		HUD->OcultarModoMultijugador();
}
