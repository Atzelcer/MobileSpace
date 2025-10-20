// Fill out your copyright notice in the Description page of Project Settings.

#include "PanelPrincipalC.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "HUDmain.h"
#include "Widget_pause.h"

void UPanelPrincipalC::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_jugar)
		Button_jugar->OnClicked.AddDynamic(this, &UPanelPrincipalC::OnJugarClicked);

	if (Button_ajustes)
		Button_ajustes->OnClicked.AddDynamic(this, &UPanelPrincipalC::OnAjustesClicked);

	if (Button_creditos)
		Button_creditos->OnClicked.AddDynamic(this, &UPanelPrincipalC::OnCreditosClicked);

	if (Button_salir)
		Button_salir->OnClicked.AddDynamic(this, &UPanelPrincipalC::OnSalirClicked);
}

void UPanelPrincipalC::OnJugarClicked()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		AHUDmain* HUD = Cast<AHUDmain>(PlayerController->GetHUD());
		if (HUD)
		{
			//RemoveFromParent();
			HUD->MostrarModoJuego();
		}
	}

	//UGameplayStatics::OpenLevel(GetWorld(), FName("StarterMap"));
}


void UPanelPrincipalC::OnAjustesClicked()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		AHUDmain* HUD = Cast<AHUDmain>(PlayerController->GetHUD());
		if (HUD)
		{
			//RemoveFromParent();
			HUD->MostrarAjustes();	
		}
	}
}


void UPanelPrincipalC::OnCreditosClicked()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		AHUDmain* HUD = Cast<AHUDmain>(PlayerController->GetHUD());
		if (HUD)
		{
			//RemoveFromParent();
			HUD->MostrarCreditos();
		}
	}
}

void UPanelPrincipalC::OnSalirClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, true);
}
