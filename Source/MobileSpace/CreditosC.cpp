// Fill out your copyright notice in the Description page of Project Settings.


#include "CreditosC.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "HUDmain.h"

void UCreditosC::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_volver)
		Button_volver->OnClicked.AddDynamic(this, &UCreditosC::OnVolverClicked);
}

void UCreditosC::OnVolverClicked()
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