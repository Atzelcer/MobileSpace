// Fill out your copyright notice in the Description page of Project Settings.
// HUDmain.cpp
#include "HUDmain.h"
#include "PanelPrincipalC.h"
#include "ModoJuegoC.h"
#include "AjustesC.h"
#include "CreditosC.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "WidgetPantallaCarga.h"
#include "Widget_ON_GAME.h"
#include "Widget_pause.h"

AHUDmain::AHUDmain()
{
	static ConstructorHelpers::FClassFinder<UPanelPrincipalC> PanelPrincipalBPClass(TEXT("/Game/WIDGETS/PanelPrincipal.PanelPrincipal_C"));
	if (PanelPrincipalBPClass.Succeeded())
		PanelPrincipalClass = PanelPrincipalBPClass.Class;


	static ConstructorHelpers::FClassFinder<UModoJuegoC> ModoJuegoBPClass(TEXT("/Game/WIDGETS/ModoJuego.ModoJuego_C"));
	if (ModoJuegoBPClass.Succeeded())
		ModoJuegoClass = ModoJuegoBPClass.Class;


	static ConstructorHelpers::FClassFinder<UAjustesC> AjustesBPClass(TEXT("/Game/WIDGETS/Ajustes.Ajustes_C"));
	if (AjustesBPClass.Succeeded())
		AjustesClass = AjustesBPClass.Class;


	static ConstructorHelpers::FClassFinder<UCreditosC> CreditosBPClass(TEXT("/Game/WIDGETS/Creditos.Creditos_C"));
	if (CreditosBPClass.Succeeded())
		CreditosClass = CreditosBPClass.Class;


	static ConstructorHelpers::FClassFinder<UWidgetPantallaCarga> PantallaCargaBPClass(TEXT("/Game/WIDGETS/ModoCarga.ModoCarga_C"));
	if (PantallaCargaBPClass.Succeeded())
		PantallaCargaClass = PantallaCargaBPClass.Class;


	static ConstructorHelpers::FClassFinder<UWidget_ON_GAME> OnGameBPClass(TEXT("/Game/WIDGETS/EnGame.EnGame_C"));
	if (OnGameBPClass.Succeeded())
		WidgetOnGameClass = OnGameBPClass.Class;


	static ConstructorHelpers::FClassFinder<UWidget_pause> PauseBPClass(TEXT("/Game/WIDGETS/MenuPausa.MenuPausa_C"));
	if (PauseBPClass.Succeeded())
		WidgetPauseClass = PauseBPClass.Class;

	static ConstructorHelpers::FObjectFinder<USoundBase> MusicaAsset(TEXT("SoundWave'/Game/AuroraSoundTrack/Wav/Cosmic_Horizons.Cosmic_Horizons'"));
	if (MusicaAsset.Succeeded())
		MusicaInicio = MusicaAsset.Object;
}

void AHUDmain::BeginPlay()
{
	Super::BeginPlay();
	MostrarPanelPrincipal();
}

void AHUDmain::MostrarPanelPrincipal()
{
	if (!PanelPrincipalInstance && PanelPrincipalClass)
		PanelPrincipalInstance = CreateWidget<UPanelPrincipalC>(GetWorld(), PanelPrincipalClass);

	if (PanelPrincipalInstance && !PanelPrincipalInstance->IsInViewport())
	{
		PanelPrincipalInstance->AddToViewport();
		ReproducirMusicaInicio();
		ConfigurarInputController();
	}
}

void AHUDmain::OcultarPanelPrincipal()
{
	if (PanelPrincipalInstance && PanelPrincipalInstance->IsInViewport())
	{
		PanelPrincipalInstance->RemoveFromParent();
		//DetenerMusicaInicio();
		//RemoverInputController();
	}
}

void AHUDmain::MostrarModoJuego()
{
	if (!ModoJuegoInstance && ModoJuegoClass)
		ModoJuegoInstance = CreateWidget<UModoJuegoC>(GetWorld(), ModoJuegoClass);

	if (ModoJuegoInstance && !ModoJuegoInstance->IsInViewport())
		ModoJuegoInstance->AddToViewport();
}

void AHUDmain::OcultarModoJuego()
{
	if (ModoJuegoInstance && ModoJuegoInstance->IsInViewport())
		ModoJuegoInstance->RemoveFromParent();
}

void AHUDmain::MostrarAjustes()
{
	if (!AjustesInstance && AjustesClass)
		AjustesInstance = CreateWidget<UAjustesC>(GetWorld(), AjustesClass);

	if (AjustesInstance && !AjustesInstance->IsInViewport())
		AjustesInstance->AddToViewport();
}

void AHUDmain::OcultarAjustes()
{
	if (AjustesInstance && AjustesInstance->IsInViewport())
		AjustesInstance->RemoveFromParent();
}

void AHUDmain::MostrarCreditos()
{
	if (!CreditosInstance && CreditosClass)
		CreditosInstance = CreateWidget<UCreditosC>(GetWorld(), CreditosClass);

	if (CreditosInstance && !CreditosInstance->IsInViewport())
		CreditosInstance->AddToViewport();
}

void AHUDmain::OcultarCreditos()
{
	if (CreditosInstance && CreditosInstance->IsInViewport())
		CreditosInstance->RemoveFromParent();
}


void AHUDmain::MostrarPantallaCarga()
{
	if (!PantallaCargaInstance && PantallaCargaClass)
	{
		PantallaCargaInstance = CreateWidget<UWidgetPantallaCarga>(GetWorld(), PantallaCargaClass);
	}

	if (PantallaCargaInstance && !PantallaCargaInstance->IsInViewport())
	{
		PantallaCargaInstance->AddToViewport(5);
		PantallaCargaInstance->MostrarFondoAleatorio();
		UGameplayStatics::SetGamePaused(GetWorld(), false);
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle_OcultarPantallaCarga,
			this,
			&AHUDmain::OcultarPantallaCarga,
			5.0f,
			false
		);
	}
}


void AHUDmain::OcultarPantallaCarga()
{
	if (PantallaCargaInstance && PantallaCargaInstance->IsInViewport())
		PantallaCargaInstance->RemoveFromParent();

	DetenerMusicaInicio();
	RemoverInputController();
	MostrarOnGame();
}

void AHUDmain::MostrarOnGame()
{
	if (!WidgetOnGameInstance && WidgetOnGameClass)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		WidgetOnGameInstance = CreateWidget<UWidget_ON_GAME>(PC, WidgetOnGameClass);
	}

	if (WidgetOnGameInstance && !WidgetOnGameInstance->IsInViewport())
	{
		WidgetOnGameInstance->AddToViewport();

		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PC)
		{
			UGameplayStatics::SetGamePaused(GetWorld(), false);

			// Mostrar cursor pero mantener control del juego
			PC->bShowMouseCursor = true;
			FInputModeGameAndUI InputMode;
			InputMode.SetWidgetToFocus(WidgetOnGameInstance->TakeWidget());
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PC->SetInputMode(InputMode);
		}
	}
}


void AHUDmain::OcultarOnGame()
{
	if (WidgetOnGameInstance && WidgetOnGameInstance->IsInViewport())
	{
		WidgetOnGameInstance->RemoveFromParent();
	}
}

void AHUDmain::MostrarPause()
{
	if (!WidgetPauseInstance && WidgetPauseClass)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		WidgetPauseInstance = CreateWidget<UWidget_pause>(PC, WidgetPauseClass);
	}

	if (WidgetPauseInstance && !WidgetPauseInstance->IsInViewport())
	{
		WidgetPauseInstance->AddToViewport();
		UGameplayStatics::SetGamePaused(GetWorld(), true);
	}
}

void AHUDmain::OcultarPause()
{
	if (WidgetPauseInstance && WidgetPauseInstance->IsInViewport())
	{
		WidgetPauseInstance->RemoveFromParent();
		UGameplayStatics::SetGamePaused(GetWorld(), false);
	}
}

void AHUDmain::ReproducirMusicaInicio()
{
	if (MusicaInicio)
		MusicaComponent = UGameplayStatics::SpawnSound2D(GetWorld(), MusicaInicio, 0.6f, 1.0f, 0.0f, nullptr, true);
}

void AHUDmain::DetenerMusicaInicio()
{
	if (MusicaComponent && MusicaComponent->IsPlaying())
		MusicaComponent->FadeOut(1.0f, 0.0f);
}

void AHUDmain::ConfigurarInputController()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		PlayerController->bShowMouseCursor = true;
		PlayerController->SetInputMode(FInputModeUIOnly());
	}
}

void AHUDmain::RemoverInputController()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		UGameplayStatics::SetGamePaused(GetWorld(), false);
		PlayerController->bShowMouseCursor = false;
		PlayerController->SetInputMode(FInputModeGameOnly());
	}
}

void AHUDmain::OcultarTodo()
{
	OcultarPanelPrincipal();
	OcultarModoJuego();
	OcultarAjustes();
	OcultarCreditos();
}
