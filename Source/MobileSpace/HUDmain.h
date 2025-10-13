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

	// Widget Panel Principal


public:

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UPanelPrincipalC> PanelPrincipalClass;

	UPROPERTY()
	class UPanelPrincipalC* PanelPrincipalInstance;

	// Widget Modo Juego

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UModoJuegoC> ModoJuegoClass;

	UPROPERTY()
	class UModoJuegoC* ModoJuegoInstance;


	// Widget Ajustes
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UAjustesC> AjustesClass;

	UPROPERTY()
	class UAjustesC* AjustesInstance;

	// Widget Creditos
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UCreditosC> CreditosClass;

	UPROPERTY()
	class UCreditosC* CreditosInstance;


	// Audio General

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	class USoundBase* MusicaInicio;

	UPROPERTY()
	class UAudioComponent* MusicaComponent;

public:

	UFUNCTION(BlueprintCallable, Category = "Audio")
	void ReproducirMusicaInicio();

	UFUNCTION(BlueprintCallable, Category = "Audio")
	void DetenerMusicaInicio();\

	// Configuracion UI Controller
	UFUNCTION(BlueprintCallable, Category = "controllers")
	void ConfigurarInputController();

	UFUNCTION(BlueprintCallable, Category = "controllers")
	void RemoverInputController();


	UFUNCTION(BlueprintCallable, Category = "UI")
	void MostrarPanelPrincipal();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void OcultarPanelPrincipal();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void MostrarModoJuego();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void OcultarModoJuego();


	UFUNCTION(BlueprintCallable, Category = "UI")
	void MostrarAjustes();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void OcultarAjustes();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void MostrarCreditos();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void OcultarCreditos();

};
