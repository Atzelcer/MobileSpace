// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget_ON_GAME.h"
#include "Kismet/GameplayStatics.h"
#include "HUDmain.h"

void UWidget_ON_GAME::NativeConstruct()
{
	Super::NativeConstruct();

	bEscudoActivo = true;
	bVelocidadActiva = true;

	TexEscudo_ON = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/Game/Imagenes/botones/escudo.escudo'"));
	TexEscudo_OFF = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/Game/Imagenes/botones/escudo_off.escudo_off'"));
	TexVelocidad_ON = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/Game/Imagenes/botones/ONvelocidad.ONvelocidad'"));
	TexVelocidad_OFF = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/Game/Imagenes/botones/ONvelocidad_off.ONvelocidad_off'"));

	if (Button_escudo) Button_escudo->OnClicked.AddDynamic(this, &UWidget_ON_GAME::OnEscudoClicked);
	if (Button_velocidad) Button_velocidad->OnClicked.AddDynamic(this, &UWidget_ON_GAME::OnVelocidadClicked);
	if (Button_missil) Button_missil->OnClicked.AddDynamic(this, &UWidget_ON_GAME::OnMissilClicked);

	ActualizarEstadoBotones();
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

void UWidget_ON_GAME::ActualizarSobrecarga(float Valor, float MaxValor)
{
	if (ProgressBar_Sobrecarga_Disparo)
	{
		float Progreso = FMath::Clamp(Valor / MaxValor, 0.f, 1.f);
		ProgressBar_Sobrecarga_Disparo->SetPercent(Progreso);
	}
}

void UWidget_ON_GAME::CambiarArma(UTexture2D* NuevaArma)
{
	if (image_arma_cambio && NuevaArma)
		image_arma_cambio->SetBrushFromTexture(NuevaArma);
}

void UWidget_ON_GAME::ActualizarEstadoBotones()
{
	if (Image_ESCUDO)
		Image_ESCUDO->SetBrushFromTexture(bEscudoActivo ? TexEscudo_ON : TexEscudo_OFF);

	if (Image_VELOCIDAD)
		Image_VELOCIDAD->SetBrushFromTexture(bVelocidadActiva ? TexVelocidad_ON : TexVelocidad_OFF);
}

void UWidget_ON_GAME::OnEscudoClicked()
{
	if (!bEscudoActivo) return;

	int32 CantidadActual = FCString::Atoi(*TextBlock_cantidad_escudo->GetText().ToString());
	if (CantidadActual > 0)
	{
		CantidadActual--;
		TextBlock_cantidad_escudo->SetText(FText::AsNumber(CantidadActual));
		bEscudoActivo = false;
		ActualizarEstadoBotones();
	}
}

void UWidget_ON_GAME::OnVelocidadClicked()
{
	if (!bVelocidadActiva) return;

	int32 CantidadActual = FCString::Atoi(*TextBlock_cantidad_velocidad->GetText().ToString());
	if (CantidadActual > 0)
	{
		CantidadActual--;
		TextBlock_cantidad_velocidad->SetText(FText::AsNumber(CantidadActual));
		bVelocidadActiva = false;
		ActualizarEstadoBotones();
	}
}

void UWidget_ON_GAME::OnMissilClicked()
{
	int32 CantidadActual = FCString::Atoi(*TextBlock_cantidad_misil->GetText().ToString());
	if (CantidadActual > 0)
	{
		CantidadActual--;
		TextBlock_cantidad_misil->SetText(FText::AsNumber(CantidadActual));
	}
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