// Fill out your copyright notice in the Description page of Project Settings.


#include "ModoJuegoC.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "HUDmain.h"
#include "CreditosC.h"

void UModoJuegoC::NativeConstruct()
{
    Super::NativeConstruct();

    if (Button_volver)
        Button_volver->OnClicked.AddDynamic(this, &UModoJuegoC::OnVolverClicked);

    if (Button_multijugador)
        Button_multijugador->OnClicked.AddDynamic(this, &UModoJuegoC::OnMultijugadorClicked);

    if (Button_aventura)
        Button_aventura->OnClicked.AddDynamic(this, &UModoJuegoC::OnAventuraClicked);
}


void UModoJuegoC::OnAventuraClicked()
{

    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PC)
    {
        AHUDmain* HUD = Cast<AHUDmain>(PC->GetHUD());
        if (HUD)
        {
			HUD->MostrarPantallaCarga();
			HUD->OcultarPanelPrincipal();
            HUD->OcultarModoJuego();
			/*HUD->DetenerMusicaInicio();
            HUD->RemoverInputController();*/
        }
    }
    //UGameplayStatics::OpenLevel(GetWorld(), FName("MapaAventura"));
}


void UModoJuegoC::OnMultijugadorClicked()
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PC)
    {
        AHUDmain* HUD = Cast<AHUDmain>(PC->GetHUD());
        if (HUD)
        {
            HUD->MostrarModoMultijugador();
            //HUD->OcultarPanelPrincipal();
            //HUD->OcultarModoJuego();

        }
    }
}



void UModoJuegoC::OnVolverClicked()
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PC)
    {
        AHUDmain* HUD = Cast<AHUDmain>(PC->GetHUD());
        if (HUD)
        {
            HUD->MostrarPanelPrincipal();
            RemoveFromParent();
        }
    }
}



