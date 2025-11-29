// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetGameOver.h"
#include "Kismet/GameplayStatics.h"
#include "HUDmain.h"
#include "Engine/Engine.h"

void UWidgetGameOver::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_volvera_empezar)
		Button_volvera_empezar->OnClicked.AddDynamic(this, &UWidgetGameOver::OnVolverEmpezarClicked);

	if (Button_salir_panel_principal)
		Button_salir_panel_principal->OnClicked.AddDynamic(this, &UWidgetGameOver::OnSalirPanelPrincipalClicked);

}

void UWidgetGameOver::OnVolverEmpezarClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(*GetWorld()->GetName()), true);
}

void UWidgetGameOver::OnSalirPanelPrincipalClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(*GetWorld()->GetName()), true);

}
