// HUDmain.h
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

public:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UPanelPrincipalC> PanelPrincipalClass;

	UPROPERTY(Transient)
	class UPanelPrincipalC* PanelPrincipalInstance;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UModoJuegoC> ModoJuegoClass;

	UPROPERTY(Transient)
	class UModoJuegoC* ModoJuegoInstance;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UAjustesC> AjustesClass;

	UPROPERTY(Transient)
	class UAjustesC* AjustesInstance;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UCreditosC> CreditosClass;

	UPROPERTY(Transient)
	class UCreditosC* CreditosInstance;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	class USoundBase* MusicaInicio;

	UPROPERTY(Transient)
	class UAudioComponent* MusicaComponent;

	UFUNCTION(BlueprintCallable, Category = "Audio")
	void ReproducirMusicaInicio();

	UFUNCTION(BlueprintCallable, Category = "Audio")
	void DetenerMusicaInicio();

	UFUNCTION(BlueprintCallable, Category = "Controllers")
	void ConfigurarInputController();

	UFUNCTION(BlueprintCallable, Category = "Controllers")
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

	UFUNCTION(BlueprintCallable, Category = "UI")
	void OcultarTodo();
};
