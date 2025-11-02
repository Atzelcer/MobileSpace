// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Widget_Indicar_level.generated.h"

UCLASS()
class MOBILESPACE_API UWidget_Indicar_level : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Mensaje_Pantalla_Level;

	UPROPERTY(meta = (BindWidget))
	UImage* Colors_Change;

	UPROPERTY()
	TArray<UMaterialInterface*> BackgroundMaterials;

	UFUNCTION(BlueprintCallable)
	void CambiarColorAleatorio();

	UFUNCTION(BlueprintCallable)
	void MostrarMensaje(const FString& Mensaje);
};
