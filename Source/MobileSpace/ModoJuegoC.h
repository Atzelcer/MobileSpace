// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ModoJuegoC.generated.h"

UCLASS()
class MOBILESPACE_API UModoJuegoC : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;

private:
    UFUNCTION()
    void OnVolverClicked();

    UFUNCTION()
    void OnMultijugadorClicked();

    UFUNCTION()
    void OnAventuraClicked();

public:
    UPROPERTY(meta = (BindWidget))
    class UButton* Button_volver;

    UPROPERTY(meta = (BindWidget))
    class UButton* Button_multijugador;

    UPROPERTY(meta = (BindWidget))
    class UButton* Button_aventura;
};
