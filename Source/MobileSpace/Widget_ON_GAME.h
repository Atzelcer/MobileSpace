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

	UPROPERTY(meta = (BindWidget)) UProgressBar* ProgressBar_Sobrecarga_Disparo;
	UPROPERTY(meta = (BindWidget)) UTextBlock* TextBlock_cantidad_vida;
	UPROPERTY(meta = (BindWidget)) UTextBlock* TextBlock_cantidad_velocidad;
	UPROPERTY(meta = (BindWidget)) UImage* image_arma_cambio;
	UPROPERTY(meta = (BindWidget)) UTextBlock* TextBlock_cantidad_misil;
	UPROPERTY(meta = (BindWidget)) UTextBlock* TextBlock_cantidad_escudo;
	UPROPERTY(meta = (BindWidget)) UButton* Button_menu;

	UPROPERTY(meta = (BindWidget)) UButton* Button_escudo;
	UPROPERTY(meta = (BindWidget)) UButton* Button_velocidad;
	UPROPERTY(meta = (BindWidget)) UButton* Button_missil;

	UPROPERTY(meta = (BindWidget)) UImage* Image_ESCUDO;
	UPROPERTY(meta = (BindWidget)) UImage* Image_VELOCIDAD;

	bool bEscudoActivo;
	bool bVelocidadActiva;

	UPROPERTY() UTexture2D* TexEscudo_ON;
	UPROPERTY() UTexture2D* TexEscudo_OFF;
	UPROPERTY() UTexture2D* TexVelocidad_ON;
	UPROPERTY() UTexture2D* TexVelocidad_OFF;

	UFUNCTION() void OnMenuClicked();
	UFUNCTION(BlueprintCallable) void ActualizarVida(int32 Valor);
	UFUNCTION(BlueprintCallable) void ActualizarVelocidad(float Velocidad);
	UFUNCTION(BlueprintCallable) void ActualizarMisiles(int32 Cantidad);
	UFUNCTION(BlueprintCallable) void ActualizarEscudo(int32 Cantidad);
	UFUNCTION(BlueprintCallable) void ActualizarSobrecarga(float Valor, float MaxValor);
	UFUNCTION(BlueprintCallable) void CambiarArma(UTexture2D* NuevaArma);


	UFUNCTION() void OnEscudoClicked();
	UFUNCTION() void OnVelocidadClicked();
	UFUNCTION() void OnMissilClicked();

	void ActualizarEstadoBotones();
};
