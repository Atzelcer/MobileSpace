// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HUDmain.generated.h"

UCLASS()
class MOBILESPACE_API AHUDmain : public AHUD
{
	GENERATED_BODY()

public:
	AHUDmain();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UPanelPrincipalC> PanelPrincipalClass;

	UPROPERTY()
	class UPanelPrincipalC* PanelPrincipalInstance;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	class USoundBase* MusicaInicio;

	UPROPERTY()
	class UAudioComponent* MusicaComponent;

public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	void MostrarPanelPrincipal();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void OcultarPanelPrincipal();

	UFUNCTION(BlueprintCallable, Category = "Audio")
	void DetenerMusicaInicio();
};
