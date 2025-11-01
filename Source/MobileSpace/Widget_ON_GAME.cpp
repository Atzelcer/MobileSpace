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

void UWidget_ON_GAME::ActualizarVida(int32 Valor)
{
	if (TextBlock_cantidad_vida)
		TextBlock_cantidad_vida->SetText(FText::FromString(FString::Printf(TEXT("%d"), Valor)));
}


void UWidget_ON_GAME::ActualizarVelocidad(float Velocidad)
{
	if (TextBlock_cantidad_velocidad)
		TextBlock_cantidad_velocidad->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Velocidad)));
}

void UWidget_ON_GAME::ActualizarMisiles(int32 Cantidad)
{
	if (TextBlock_cantidad_misil)
		TextBlock_cantidad_misil->SetText(FText::FromString(FString::Printf(TEXT("%d"), Cantidad)));
}

void UWidget_ON_GAME::ActualizarEscudo(int32 Cantidad)
{
	if (TextBlock_cantidad_escudo)
		TextBlock_cantidad_escudo->SetText(FText::FromString(FString::Printf(TEXT("%d"), Cantidad)));
}

void UWidget_ON_GAME::ActualizarSobrecarga(float Progreso)
{
	if (ProgressBar_Sobrecarga_Disparo)
		ProgressBar_Sobrecarga_Disparo->SetPercent(FMath::Clamp(Progreso, 0.f, 1.f));
}

void UWidget_ON_GAME::CambiarArma(UTexture2D* NuevaArma)
{
	if (image_arma_cambio && NuevaArma)
		image_arma_cambio->SetBrushFromTexture(NuevaArma);
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