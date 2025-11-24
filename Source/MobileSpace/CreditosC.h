// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CreditosC.generated.h"

UCLASS()
class MOBILESPACE_API UCreditosC : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_volver;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* V_Creditos_A;

	UFUNCTION()
	void OnVolverClicked();

private:
	float ScrollSpeed;
	float StartY;
	float ResetOffset;
	bool bInitialized;
};
