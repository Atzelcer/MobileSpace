// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "WidgetGameOver.generated.h"


UCLASS()
class MOBILESPACE_API UWidgetGameOver : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UButton* Button_volvera_empezar;

	UPROPERTY(meta = (BindWidget))
	UButton* Button_salir_panel_principal;

	UFUNCTION()
	void OnVolverEmpezarClicked();

	UFUNCTION()
	void OnSalirPanelPrincipalClicked();

};
