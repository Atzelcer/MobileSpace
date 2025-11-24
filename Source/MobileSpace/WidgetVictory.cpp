// Fill out your copyright notice in the Description page of Project Settings.

#include "WidgetVictory.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UWidgetVictory::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_salir)
	{
		Button_salir->OnClicked.AddDynamic(this, &UWidgetVictory::OnSalirClicked);
	}
}

void UWidgetVictory::OnSalirClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainMenu"));
}
