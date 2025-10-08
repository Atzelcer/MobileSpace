// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PanelPrincipalC.generated.h"

UCLASS()
class MOBILESPACE_API UPanelPrincipalC : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_jugar;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_ajustes;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_creditos;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_salir;

	UFUNCTION()
	void OnJugarClicked();

	UFUNCTION()
	void OnAjustesClicked();

	UFUNCTION()
	void OnCreditosClicked();

	UFUNCTION()
	void OnSalirClicked();
};
