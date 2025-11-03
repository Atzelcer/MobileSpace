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

	// --- Widgets principales ---
	UPROPERTY(meta = (BindWidget)) UProgressBar* ProgressBar_Sobrecarga_Disparo;
	UPROPERTY(meta = (BindWidget)) UTextBlock* TextBlock_cantidad_vida;
	UPROPERTY(meta = (BindWidget)) UTextBlock* TextBlock_cantidad_velocidad;
	UPROPERTY(meta = (BindWidget)) UTextBlock* TextBlock_cantidad_misil;
	UPROPERTY(meta = (BindWidget)) UTextBlock* TextBlock_cantidad_escudo;
	UPROPERTY(meta = (BindWidget)) UImage* Image_ESCUDO;
	UPROPERTY(meta = (BindWidget)) UImage* Image_VELOCIDAD;
	UPROPERTY(meta = (BindWidget)) UImage* Image_Missil;
	UPROPERTY(meta = (BindWidget)) UImage* image_arma_cambio;
	UPROPERTY(meta = (BindWidget)) UButton* Button_menu;
	UPROPERTY(meta = (BindWidget)) UButton* Button_escudo;
	UPROPERTY(meta = (BindWidget)) UButton* Button_velocidad;
	UPROPERTY(meta = (BindWidget)) UButton* Button_missil;
	UPROPERTY(meta = (BindWidget)) UImage* Image_panel01;
	UPROPERTY(meta = (BindWidget)) UImage* Image_panel02;
	UPROPERTY(meta = (BindWidget)) UImage* Image_panel03;

	// --- Estados ---
	bool bEscudoActivo;
	bool bVelocidadActiva;

	// --- Texturas principales ---
	UPROPERTY() UTexture2D* TexEscudo_ON;
	UPROPERTY() UTexture2D* TexEscudo_OFF;
	UPROPERTY() UTexture2D* TexVelocidad_ON;
	UPROPERTY() UTexture2D* TexVelocidad_OFF;
	UPROPERTY() UTexture2D* ONmissil;
	UPROPERTY() UTexture2D* ONmissil_off;

	// --- Listas de texturas para paneles (corrección del error E0020) ---
	TArray<UTexture2D*> PanelGameTextures;
	TArray<UTexture2D*> IconHamburTextures;

	// --- Métodos funcionales ---
	void CargarTexturasPanels();
	void CambiarPanelsAleatorios();
	void ActualizarEstadoBotones();
	void ActualizarVida(int32 Valor);
	void ActualizarVelocidad(float Velocidad);
	void ActualizarMisiles(int32 Cantidad);
	void ActualizarEscudo(int32 Cantidad);
	void ActualizarSobrecarga(float Valor, float MaxValor);
	void CambiarArma(UTexture2D* NuevaArma);

	// --- Botones ---
	UFUNCTION() void OnEscudoClicked();
	UFUNCTION() void OnVelocidadClicked();
	UFUNCTION() void OnMissilClicked();
	UFUNCTION() void OnMenuClicked();

	// --- Reactivaciones controladas desde el Pawn ---
	void ReactivarEscudo();
	void ReactivarVelocidad();
};
