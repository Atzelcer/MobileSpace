// HUDmain.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Widget_Indicar_level.h"
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

	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void ModoAventura();

	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void ModoMultijugador();

private:

	UPROPERTY()
	class AFacadeGameManager* FacadeRef;

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

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UWidgetPantallaCarga> PantallaCargaClass;

	UPROPERTY(Transient)
	class UWidgetPantallaCarga* PantallaCargaInstance;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UWidget_ON_GAME> WidgetOnGameClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UWidget_Modo_multijugador> ModoMultijugadorClass;

	UPROPERTY(Transient)
	class UWidget_Modo_multijugador* ModoMultijugadorInstance;

	UPROPERTY(Transient)
	class UWidget_ON_GAME* WidgetOnGameInstance;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UWidget_pause> WidgetPauseClass;

	UPROPERTY(Transient)
	class UWidget_pause* WidgetPauseInstance;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	class TSubclassOf<class UWidgetSalaEspera> WidgetSalaEsperaClass;

	UPROPERTY(Transient)
	class UWidgetSalaEspera* WidgetSalaEsperaInstance;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	class TSubclassOf<class UWidgetPCMulti> WidgetPCMultiClass;

	UPROPERTY(Transient)
	class UWidgetPCMulti* WidgetPCMultiInstance;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	class TSubclassOf<class UWidgetOnGameMulti> WidgetOnGameMultiClass;

	UPROPERTY(Transient)
	class UWidgetOnGameMulti* WidgetOnGameMultiInstance;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	class TSubclassOf<class UWidgetGameOver> WidgetGameOverClass;

	UPROPERTY(Transient)
	class UWidgetGameOver* WidgetGameOverInstance;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	class TSubclassOf<class UWidget_Indicar_level> WidgetLevelClass;

	UPROPERTY(Transient)
	class UWidget_Indicar_level* WidgetLevelInstance;

	UPROPERTY(Transient)
	class UAudioComponent* MusicaComponent;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UWidgetMegaBoss> WidgetMegaBossClass;

	UPROPERTY(Transient)
	class UWidgetMegaBoss* WidgetMegaBossInstance;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<class UWidgetVictory> WidgetVictoryClass;

	UPROPERTY(Transient)
	class UWidgetVictory* WidgetVictoryInstance;


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
	void MostrarPantallaCarga();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void OcultarPantallaCarga();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void MostrarPantallaCarga2();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void OcultarPantallaCarga2();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void MostrarOnGame();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void OcultarOnGame();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void MostrarPause();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void OcultarPause();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void MostrarModoMultijugador();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void OcultarModoMultijugador();


	UFUNCTION(BlueprintCallable)
	void MostrarSalaEspera();

	UFUNCTION(BlueprintCallable)
	void OcultarSalaEspera();

	UFUNCTION(BlueprintCallable)
	void MostrarPantallaCargaMulti();

	UFUNCTION(BlueprintCallable)
	void OcultarPantallaCargaMulti();

	UFUNCTION(BlueprintCallable)
	void MostrarOnGameMulti();

	UFUNCTION(BlueprintCallable)
	void OcultarOnGameMulti();

	UFUNCTION(BlueprintCallable)
	void MostrarGameOver();

	UFUNCTION(BlueprintCallable)
	void OcultarGameOver();

	UFUNCTION(BlueprintCallable)
	void MostrarNivel(const FString& Mensaje);

	UFUNCTION(BlueprintCallable)
	void OcultarNivel();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void MostrarVictory();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void OcultarVictory();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void OcultarTodo();

	/*UFUNCTION(BlueprintCallable)
	void MostrarMegaBoss();

	UFUNCTION(BlueprintCallable)
	void OcultarMegaBoss();*/


	FTimerHandle TimerHandle_OcultarNivel;
	FTimerHandle TimerHandle_OcultarPantallaCarga;
	FTimerHandle TimerHandle_ConteoPantallaCarga;
	int32 TiempoRestantePantallaCarga;

};
