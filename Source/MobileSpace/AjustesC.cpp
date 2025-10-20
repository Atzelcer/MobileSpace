#include "AjustesC.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Engine/PostProcessVolume.h"
#include "HUDmain.h"

void UAjustesC::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_volver)
		Button_volver->OnClicked.AddDynamic(this, &UAjustesC::OnVolverClicked);

	const float ValorInicial = 75.0f;

	// BRILLO
	if (Slider_BRILLO_AJUSTES)
	{
		Slider_BRILLO_AJUSTES->SetMinValue(1.0f);
		Slider_BRILLO_AJUSTES->SetMaxValue(100.0f);
		Slider_BRILLO_AJUSTES->SetStepSize(1.0f);
		Slider_BRILLO_AJUSTES->SetValue(ValorInicial);
		Slider_BRILLO_AJUSTES->OnValueChanged.AddDynamic(this, &UAjustesC::OnBrilloChanged);
		ActualizarColorSlider(Slider_BRILLO_AJUSTES, ValorInicial);
		if (TextBlock_BRILLO_AJUSTES)
			TextBlock_BRILLO_AJUSTES->SetText(FText::FromString(TEXT("75")));
	}

	// VOLUMEN PRINCIPAL
	if (Slider_VOLUMEN_PRINCIPAL_AJUSTES)
	{
		Slider_VOLUMEN_PRINCIPAL_AJUSTES->SetMinValue(1.0f);
		Slider_VOLUMEN_PRINCIPAL_AJUSTES->SetMaxValue(100.0f);
		Slider_VOLUMEN_PRINCIPAL_AJUSTES->SetStepSize(1.0f);
		Slider_VOLUMEN_PRINCIPAL_AJUSTES->SetValue(ValorInicial);
		Slider_VOLUMEN_PRINCIPAL_AJUSTES->OnValueChanged.AddDynamic(this, &UAjustesC::OnVolumenPrincipalChanged);
		ActualizarColorSlider(Slider_VOLUMEN_PRINCIPAL_AJUSTES, ValorInicial);
		if (TextBlock_VOLUMEN_PRINCIPAL_AJUSTES)
			TextBlock_VOLUMEN_PRINCIPAL_AJUSTES->SetText(FText::FromString(TEXT("75")));
	}

	// VOLUMEN MÚSICA
	if (Slider_VOLUMEN_MUSICA_AJUSTES)
	{
		Slider_VOLUMEN_MUSICA_AJUSTES->SetMinValue(1.0f);
		Slider_VOLUMEN_MUSICA_AJUSTES->SetMaxValue(100.0f);
		Slider_VOLUMEN_MUSICA_AJUSTES->SetStepSize(1.0f);
		Slider_VOLUMEN_MUSICA_AJUSTES->SetValue(ValorInicial);
		Slider_VOLUMEN_MUSICA_AJUSTES->OnValueChanged.AddDynamic(this, &UAjustesC::OnVolumenMusicaChanged);
		ActualizarColorSlider(Slider_VOLUMEN_MUSICA_AJUSTES, ValorInicial);
		if (TextBlock_VOLUMEN_MUSICA_AJUSTES)
			TextBlock_VOLUMEN_MUSICA_AJUSTES->SetText(FText::FromString(TEXT("75")));
	}

	// VOLUMEN EFECTOS
	if (Slider_VOLUMEN_EFECTOS_AJUSTES)
	{
		Slider_VOLUMEN_EFECTOS_AJUSTES->SetMinValue(1.0f);
		Slider_VOLUMEN_EFECTOS_AJUSTES->SetMaxValue(100.0f);
		Slider_VOLUMEN_EFECTOS_AJUSTES->SetStepSize(1.0f);
		Slider_VOLUMEN_EFECTOS_AJUSTES->SetValue(ValorInicial);
		Slider_VOLUMEN_EFECTOS_AJUSTES->OnValueChanged.AddDynamic(this, &UAjustesC::OnVolumenEfectosChanged);
		ActualizarColorSlider(Slider_VOLUMEN_EFECTOS_AJUSTES, ValorInicial);
		if (TextBlock_VOLUMEN_EFECTOS_AJUSTES)
			TextBlock_VOLUMEN_EFECTOS_AJUSTES->SetText(FText::FromString(TEXT("75")));
	}
}

void UAjustesC::OnVolverClicked()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC) return;

	AHUDmain* HUD = Cast<AHUDmain>(PC->GetHUD());
	if (HUD)
	{
		RemoveFromParent();
		//HUD->MostrarPanelPrincipal();
	}
}

void UAjustesC::OnBrilloChanged(float Value)
{
	if (TextBlock_BRILLO_AJUSTES)
		TextBlock_BRILLO_AJUSTES->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int32)Value)));

	ActualizarColorSlider(Slider_BRILLO_AJUSTES, Value);

	const float Normalized = Value / 100.0f;
	const float Bias = FMath::Lerp(-2.0f, 2.0f, Normalized);

	for (TActorIterator<APostProcessVolume> It(GetWorld()); It; ++It)
	{
		APostProcessVolume* PPV = *It;
		if (PPV && PPV->bUnbound)
		{
			PPV->Settings.bOverride_AutoExposureBias = true;
			PPV->Settings.AutoExposureBias = Bias;
			break;
		}
	}
}

void UAjustesC::OnVolumenPrincipalChanged(float Value)
{
	if (TextBlock_VOLUMEN_PRINCIPAL_AJUSTES)
		TextBlock_VOLUMEN_PRINCIPAL_AJUSTES->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int32)Value)));

	ActualizarColorSlider(Slider_VOLUMEN_PRINCIPAL_AJUSTES, Value);
}

void UAjustesC::OnVolumenMusicaChanged(float Value)
{
	if (TextBlock_VOLUMEN_MUSICA_AJUSTES)
		TextBlock_VOLUMEN_MUSICA_AJUSTES->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int32)Value)));

	ActualizarColorSlider(Slider_VOLUMEN_MUSICA_AJUSTES, Value);
}

void UAjustesC::OnVolumenEfectosChanged(float Value)
{
	if (TextBlock_VOLUMEN_EFECTOS_AJUSTES)
		TextBlock_VOLUMEN_EFECTOS_AJUSTES->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int32)Value)));

	ActualizarColorSlider(Slider_VOLUMEN_EFECTOS_AJUSTES, Value);
}

void UAjustesC::ActualizarColorSlider(USlider* Slider, float Value)
{
	if (!Slider) return;

	float Normalized = FMath::Clamp(Value / 100.0f, 0.0f, 1.0f);

	FLinearColor FillColor;
	if (Normalized < 0.33f)
		FillColor = FLinearColor::LerpUsingHSV(FLinearColor(0.0f, 0.4f, 1.0f), FLinearColor(0.0f, 1.0f, 0.0f), Normalized * 3.0f);
	else if (Normalized < 0.66f)
		FillColor = FLinearColor::LerpUsingHSV(FLinearColor(0.0f, 1.0f, 0.0f), FLinearColor(1.0f, 1.0f, 0.0f), (Normalized - 0.33f) * 3.0f);
	else
		FillColor = FLinearColor::LerpUsingHSV(FLinearColor(1.0f, 1.0f, 0.0f), FLinearColor(1.0f, 0.0f, 0.0f), (Normalized - 0.66f) * 3.0f);

	FLinearColor EmptyColor = FLinearColor(0.95f, 0.95f, 0.95f, 1.0f);

	FLinearColor Mixed = FLinearColor(
		FMath::Lerp(EmptyColor.R, FillColor.R, Normalized),
		FMath::Lerp(EmptyColor.G, FillColor.G, Normalized),
		FMath::Lerp(EmptyColor.B, FillColor.B, Normalized),
		1.0f
	);

	Slider->SetSliderHandleColor(FillColor);
	Slider->SetSliderBarColor(Mixed);
}
