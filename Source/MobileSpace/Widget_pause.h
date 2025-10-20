// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Widget_pause.generated.h"

UCLASS()
class MOBILESPACE_API UWidget_pause : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UButton* Button_CONTINUAR;

	UPROPERTY(meta = (BindWidget))
	UButton* Button_ajustes;

	UPROPERTY(meta = (BindWidget))
	UButton* Button_MENU_PRINCIPAL;

	UFUNCTION()
	void OnContinuarClicked();

	UFUNCTION()
	void OnAjustesClicked();

	UFUNCTION()
	void OnMenuPrincipalClicked();
};
