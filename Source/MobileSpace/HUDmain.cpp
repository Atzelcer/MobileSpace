// Fill out your copyright notice in the Description page of Project Settings.

#include "HUDmain.h"
#include "PanelPrincipalC.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"

AHUDmain::AHUDmain()
{
	static ConstructorHelpers::FClassFinder<UPanelPrincipalC> PanelPrincipalBPClass(TEXT("/Game/WIDGETS/PanelPrincipal.PanelPrincipal_C"));
	if (PanelPrincipalBPClass.Succeeded())
	{
		PanelPrincipalClass = PanelPrincipalBPClass.Class;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> MusicaAsset(TEXT("SoundWave'/Game/AuroraSoundTrack/Wav/Cosmic_Horizons.Cosmic_Horizons'"));
	if (MusicaAsset.Succeeded())
	{
		MusicaInicio = MusicaAsset.Object;
	}
}

void AHUDmain::BeginPlay()
{
	Super::BeginPlay();
	MostrarPanelPrincipal();
}

void AHUDmain::MostrarPanelPrincipal()
{
	if (!PanelPrincipalInstance && PanelPrincipalClass)
	{
		PanelPrincipalInstance = CreateWidget<UPanelPrincipalC>(GetWorld(), PanelPrincipalClass);
	}

	if (PanelPrincipalInstance && !PanelPrincipalInstance->IsInViewport())
	{
		PanelPrincipalInstance->AddToViewport();

		if (MusicaInicio)
		{
			MusicaComponent = UGameplayStatics::SpawnSound2D(GetWorld(), MusicaInicio, 0.6f, 1.0f, 0.0f, nullptr, true);
		}
	}
}

void AHUDmain::OcultarPanelPrincipal()
{
	if (PanelPrincipalInstance && PanelPrincipalInstance->IsInViewport())
	{
		DetenerMusicaInicio();
		PanelPrincipalInstance->RemoveFromParent();

		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PlayerController)
		{
			UGameplayStatics::SetGamePaused(GetWorld(), false);
			PlayerController->bShowMouseCursor = false;
			PlayerController->SetInputMode(FInputModeGameOnly());
		}
	}
}

void AHUDmain::DetenerMusicaInicio()
{
	if (MusicaComponent && MusicaComponent->IsPlaying())
	{
		MusicaComponent->Stop();
	}
}
