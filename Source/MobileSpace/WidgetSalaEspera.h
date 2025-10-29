// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "WidgetSalaEspera.generated.h"

UCLASS()
class MOBILESPACE_API UWidgetSalaEspera : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock_codigoSala;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_nave_Player1;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_nave_Player2;

	void ActualizarCodigo(const FString& Codigo);
	void JugadorConectado(int32 NumeroJugador);
	void IrAPantallaCarga();
};
