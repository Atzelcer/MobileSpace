// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "TimerManager.h"
#include "WidgetPantallaCarga.generated.h"


UCLASS()
class MOBILESPACE_API UWidgetPantallaCarga : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	virtual void NativeConstruct() override;

public:

	UPROPERTY(meta = (BindWidget))
	UImage* Image_carga_level;

	UPROPERTY()
	TArray<UTexture2D*> FondosCarga;

	void MostrarFondoAleatorio();
};