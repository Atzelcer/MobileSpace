// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetVictory.generated.h"

UCLASS()
class MOBILESPACE_API UWidgetVictory : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_salir;

	UFUNCTION()
	void OnSalirClicked();
};
