// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "WidgetOnGameMulti.generated.h"

UCLASS()
class MOBILESPACE_API UWidgetOnGameMulti : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	// Widgets
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ProgressBar_Vida;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* ProgressBar_Limit_Disparo;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock_cantidad_vida;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock_cantidad_velocidad;

	UPROPERTY(meta = (BindWidget))
	UImage* image_arma;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock_cantidad_misil;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock_cantidad_escudo;

	UPROPERTY(meta = (BindWidget))
	UButton* Button_menu;


	void ActualizarVida(float Valor, float MaxValor);
	void ActualizarLimiteDisparo(float Valor, float MaxValor);
	void ActualizarVelocidad(float Velocidad);
	void ActualizarMisiles(int32 Cantidad);
	void ActualizarEscudo(int32 Cantidad);
	void CambiarArma(UTexture2D* NuevaArma);


	UFUNCTION()
	void OnMenuClicked();
};
