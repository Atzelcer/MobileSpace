// Fill out your copyright notice in the Description page of Project Settings.

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/EditableTextBox.h"
#include "Widget_Modo_multijugador.generated.h"

UCLASS()
class MOBILESPACE_API UWidget_Modo_multijugador : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	// === WIDGETS ===
	UPROPERTY(meta = (BindWidget))
	UButton* Button_crear_sala;

	UPROPERTY(meta = (BindWidget))
	UButton* Button_entrar_sala;

	UPROPERTY(meta = (BindWidget))
	UButton* Button_volver;

	UPROPERTY(meta = (BindWidget))
	UButton* Button_cerrar_ventana_sala;

	UPROPERTY(meta = (BindWidget))
	UButton* Button_entrar_sala_ventana;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock_Ventana_sala;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_fondo_Ventana;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_marco_entrar;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* codigoSala_NEW;

	// === FUNCIONES ===
	void MostrarVentana(bool bMostrar);

	UFUNCTION()
	void OnCrearSalaClicked();

	UFUNCTION()
	void OnEntrarSalaClicked();

	UFUNCTION()
	void OnVolverClicked();

	UFUNCTION()
	void OnCerrarVentanaClicked();

	UFUNCTION()
	void OnEntrarSalaVentanaClicked();
};
