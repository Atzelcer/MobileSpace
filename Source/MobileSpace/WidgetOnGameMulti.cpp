// Fill out your copyright notice in the Description page of Project Settings.
#include "WidgetOnGameMulti.h"
#include "Kismet/GameplayStatics.h"
#include "HUDmain.h"

void UWidgetOnGameMulti::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_menu)
		Button_menu->OnClicked.AddDynamic(this, &UWidgetOnGameMulti::OnMenuClicked);

	if (ProgressBar_Vida)
		ProgressBar_Vida->SetPercent(1.0f);

	if (ProgressBar_Limit_Disparo)
		ProgressBar_Limit_Disparo->SetPercent(1.0f);

	if (TextBlock_cantidad_vida)
		TextBlock_cantidad_vida->SetText(FText::FromString(TEXT("100")));

	if (TextBlock_cantidad_velocidad)
		TextBlock_cantidad_velocidad->SetText(FText::FromString(TEXT("0")));

	if (TextBlock_cantidad_misil)
		TextBlock_cantidad_misil->SetText(FText::FromString(TEXT("0")));

	if (TextBlock_cantidad_escudo)
		TextBlock_cantidad_escudo->SetText(FText::FromString(TEXT("0")));
}

void UWidgetOnGameMulti::ActualizarVida(float Valor, float MaxValor)
{
	if (ProgressBar_Vida)
		ProgressBar_Vida->SetPercent(FMath::Clamp(Valor / MaxValor, 0.0f, 1.0f));

	if (TextBlock_cantidad_vida)
		TextBlock_cantidad_vida->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Valor)));
}

void UWidgetOnGameMulti::ActualizarLimiteDisparo(float Valor, float MaxValor)
{
	if (ProgressBar_Limit_Disparo)
		ProgressBar_Limit_Disparo->SetPercent(FMath::Clamp(Valor / MaxValor, 0.0f, 1.0f));
}

void UWidgetOnGameMulti::ActualizarVelocidad(float Velocidad)
{
	if (TextBlock_cantidad_velocidad)
		TextBlock_cantidad_velocidad->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Velocidad)));
}

void UWidgetOnGameMulti::ActualizarMisiles(int32 Cantidad)
{
	if (TextBlock_cantidad_misil)
		TextBlock_cantidad_misil->SetText(FText::AsNumber(Cantidad));
}

void UWidgetOnGameMulti::ActualizarEscudo(int32 Cantidad)
{
	if (TextBlock_cantidad_escudo)
		TextBlock_cantidad_escudo->SetText(FText::AsNumber(Cantidad));
}

void UWidgetOnGameMulti::CambiarArma(UTexture2D* NuevaArma)
{
	if (image_arma && NuevaArma)
	{
		FSlateBrush Brush;
		Brush.SetResourceObject(NuevaArma);
		image_arma->SetBrush(Brush);
	}
}

void UWidgetOnGameMulti::OnMenuClicked()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{
		AHUDmain* HUD = Cast<AHUDmain>(PC->GetHUD());
		if (HUD)
			HUD->MostrarPause();
	}
}
