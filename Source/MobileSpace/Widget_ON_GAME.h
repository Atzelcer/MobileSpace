// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Widget_ON_GAME.generated.h"

UCLASS()
class MOBILESPACE_API UWidget_ON_GAME : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UButton* Button_menu;

	UFUNCTION()
	void OnMenuClicked();
};
