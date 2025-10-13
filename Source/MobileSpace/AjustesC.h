// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AjustesC.generated.h"


UCLASS()
class MOBILESPACE_API UAjustesC : public UUserWidget
{
	GENERATED_BODY()
	

public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_volver;
		
	UFUNCTION()
	void OnVolverClicked();
};
