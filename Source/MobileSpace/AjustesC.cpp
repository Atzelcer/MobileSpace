// Fill out your copyright notice in the Description page of Project Settings.


#include "AjustesC.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "HUDmain.h"

void UAjustesC::NativeConstruct()
{
	Super::NativeConstruct();
	if (Button_volver)
		Button_volver->OnClicked.AddDynamic(this, &UAjustesC::OnVolverClicked);
}

void UAjustesC::OnVolverClicked()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		AHUDmain* HUD = Cast<AHUDmain>(PlayerController->GetHUD());
		if (HUD)
		{
			RemoveFromParent();
			HUD->MostrarPanelPrincipal();
		}
	}
}
