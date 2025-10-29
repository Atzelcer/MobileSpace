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
	if (Image_nave_Player2)
		Image_nave_Player2->SetVisibility(ESlateVisibility::Hidden);
}

void UWidgetSalaEspera::ActualizarCodigo(const FString& Codigo)
{
	if (TextBlock_codigoSala)
		TextBlock_codigoSala->SetText(FText::FromString(Codigo));
}

void UWidgetSalaEspera::JugadorConectado(int32 NumeroJugador)
{
	if (NumeroJugador == 2 && Image_nave_Player2)
		Image_nave_Player2->SetVisibility(ESlateVisibility::Visible);
}

void UWidgetSalaEspera::IrAPantallaCarga()
{
	RemoveFromParent();
}
