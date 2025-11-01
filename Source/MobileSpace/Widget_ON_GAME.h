// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Widget_ON_GAME.generated.h"

UCLASS()
class MOBILESPACE_API UWidget_ON_GAME : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;


	UPROPERTY(meta = (BindWidget))
	UProgressBar* ProgressBar_Sobrecarga_Disparo;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock_cantidad_vida;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock_cantidad_velocidad;

	UPROPERTY(meta = (BindWidget))
	UImage* image_arma_cambio;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock_cantidad_misil;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock_cantidad_escudo;

	UPROPERTY(meta = (BindWidget))
	UButton* Button_menu;

	UFUNCTION()
	void OnMenuClicked();

	void ActualizarVida(int32 Valor);
	void ActualizarVelocidad(float Velocidad);
	void ActualizarMisiles(int32 Cantidad);
	void ActualizarEscudo(int32 Cantidad);
	void ActualizarSobrecarga(float Progreso);
	void CambiarArma(UTexture2D* NuevaArma);
};