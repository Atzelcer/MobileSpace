// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetSalaEspera.h"
#include "Kismet/GameplayStatics.h"
#include "HUDmain.h"
#include "Engine/Engine.h"

void UWidgetSalaEspera::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_cancelar)
		Button_cancelar->OnClicked.AddDynamic(this, &UWidgetSalaEspera::CancelarEspera);

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
	if (IsInViewport())
		RemoveFromParent();
}

void UWidgetSalaEspera::CancelarEspera()
{
	UWorld* World = GetWorld();
	if (!World) return;

	APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
	if (!PC) return;

	AHUDmain* HUD = Cast<AHUDmain>(PC->GetHUD());
	if (HUD)
	{
		HUD->OcultarSalaEspera();
		HUD->MostrarModoMultijugador();

		//if (GEngine)
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Se ha cancelado la espera de jugadores."));

		//UE_LOG(LogTemp, Warning, TEXT("Llamando a LeaveLobby desde el cliente para salir de la lobby."));
	}
}
