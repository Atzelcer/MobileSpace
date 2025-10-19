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

protected:
	// Botón para volver al menú principal
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_volver;

	// Sliders de configuración
	UPROPERTY(meta = (BindWidget))
	class USlider* Slider_BRILLO_AJUSTES;

	UPROPERTY(meta = (BindWidget))
	class USlider* Slider_VOLUMEN_PRINCIPAL_AJUSTES;

	UPROPERTY(meta = (BindWidget))
	class USlider* Slider_VOLUMEN_MUSICA_AJUSTES;

	UPROPERTY(meta = (BindWidget))
	class USlider* Slider_VOLUMEN_EFECTOS_AJUSTES;

	// Textos que muestran los valores actuales
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_BRILLO_AJUSTES;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_VOLUMEN_PRINCIPAL_AJUSTES;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_VOLUMEN_MUSICA_AJUSTES;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_VOLUMEN_EFECTOS_AJUSTES;

protected:
	// Actualiza el color del slider según su valor
	void ActualizarColorSlider(class USlider* Slider, float Value);

	// Eventos
	UFUNCTION()
	void OnVolverClicked();

	UFUNCTION()
	void OnBrilloChanged(float Value);

	UFUNCTION()
	void OnVolumenPrincipalChanged(float Value);

	UFUNCTION()
	void OnVolumenMusicaChanged(float Value);

	UFUNCTION()
	void OnVolumenEfectosChanged(float Value);
};
