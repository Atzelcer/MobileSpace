// Fill out your copyright notice in the Description page of Project Settings.

#include "WidgetSalaEspera.h"
#include "Kismet/GameplayStatics.h"
#include "HUDmain.h"
#include "Engine/Engine.h"
#include "Misc/OutputDeviceNull.h"
#include "GameFramework/GameModeBase.h"


void UWidgetSalaEspera::NativeConstruct()
{
	Super::NativeConstruct();

	FString CodigoGenerado = GenerarCodigoSala();
	ActualizarCodigo(CodigoGenerado);

	if (Image_nave_Player1) Image_nave_Player1->SetVisibility(ESlateVisibility::Visible);
	if (Image_nave_Player2) Image_nave_Player2->SetVisibility(ESlateVisibility::Hidden);
}

void UWidgetSalaEspera::ActualizarCodigo(const FString& Codigo)
{
	if (TextBlock_codigoSala)
		TextBlock_codigoSala->SetText(FText::FromString(Codigo));
}

void UWidgetSalaEspera::JugadorConectado(int32 NumeroJugador)
{
	switch (NumeroJugador)
	{
	case 1:
		if (Image_nave_Player1) Image_nave_Player1->SetVisibility(ESlateVisibility::Visible);
		break;
	case 2:
		if (Image_nave_Player2) Image_nave_Player2->SetVisibility(ESlateVisibility::Visible);
		break;
	default:
		break;
	}
}

void UWidgetSalaEspera::IrAPantallaCarga()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC) return;

	AHUDmain* HUD = Cast<AHUDmain>(PC->GetHUD());
	if (HUD)
		HUD->MostrarPantallaCargaMulti();
}

FString UWidgetSalaEspera::GenerarCodigoSala()
{
	const int32 Codigo = FMath::RandRange(1000, 9999);
	return FString::Printf(TEXT("%d"), Codigo);
}
