// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetPCMulti.generated.h"

UCLASS()
class MOBILESPACE_API UWidgetPCMulti : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

private:
	void IrAlJuego();
};
