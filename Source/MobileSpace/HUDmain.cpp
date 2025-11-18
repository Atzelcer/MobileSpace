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
#include "Widget_Modo_multijugador.h"
#include "WidgetSalaEspera.h"
#include "WidgetPCMulti.h"
#include "WidgetOnGameMulti.h"
#include "FacadeGameManager.h"
#include "EngineUtils.h"
#include "AventuraManager.h"
#include "MultiplayerManager.h"
#include "MobileSpacePawn.h"
#include "Widget_Indicar_level.h"
#include "WidgetGameOver.h"

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

	static ConstructorHelpers::FClassFinder<UWidget_Modo_multijugador> ModoMultijugadorBPClass(TEXT("/Game/WIDGETS/PanelMultijugador.PanelMultijugador_C"));
	if (ModoMultijugadorBPClass.Succeeded())
		ModoMultijugadorClass = ModoMultijugadorBPClass.Class;

	static ConstructorHelpers::FClassFinder<UWidgetSalaEspera> SalaEsperaBP(TEXT("/Game/WIDGETS/SalaEspera.SalaEspera_C"));
	if (SalaEsperaBP.Succeeded())
		WidgetSalaEsperaClass = SalaEsperaBP.Class;

	static ConstructorHelpers::FClassFinder<UWidgetPCMulti> PCMultiBP(TEXT("/Game/WIDGETS/PantallaCargaMulti.PantallaCargaMulti_C"));
	if (PCMultiBP.Succeeded())
		WidgetPCMultiClass = PCMultiBP.Class;

	static ConstructorHelpers::FClassFinder<UWidgetOnGameMulti> OnGameMultiBP(TEXT("/Game/WIDGETS/EnGame_multijugador.EnGame_multijugador_C"));
	if (OnGameMultiBP.Succeeded())
		WidgetOnGameMultiClass = OnGameMultiBP.Class;

	static ConstructorHelpers::FClassFinder<UWidget_Indicar_level> LevelWidgetBP(TEXT("/Game/WIDGETS/IndicadorLevel.IndicadorLevel_C"));
	if (LevelWidgetBP.Succeeded())
		WidgetLevelClass = LevelWidgetBP.Class;

	//para el panel de Game over
	static ConstructorHelpers::FClassFinder<UWidgetGameOver> GameOverBPClass(TEXT("/Game/WIDGETS/GameOver.GameOver_C"));
	if (GameOverBPClass.Succeeded())
		WidgetGameOverClass = GameOverBPClass.Class;

	static ConstructorHelpers::FObjectFinder<USoundBase> MusicaAsset(TEXT("SoundWave'/Game/AuroraSoundTrack/Wav/Cosmic_Horizons.Cosmic_Horizons'"));
	if (MusicaAsset.Succeeded())
		MusicaInicio = MusicaAsset.Object;
}

void AHUDmain::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (!World) return;

	for (TActorIterator<AFacadeGameManager> It(World); It; ++It)
	{
		FacadeRef = *It;
		break;
	}

	if (!FacadeRef)
	{
		FActorSpawnParameters Params;
		Params.Owner = this;
		FacadeRef = World->SpawnActor<AFacadeGameManager>(
			AFacadeGameManager::StaticClass(),
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			Params
		);
	}

	MostrarPanelPrincipal();
}


void AHUDmain::ModoAventura()
{
	if (!FacadeRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("No existe referencia a FacadeGameManager."));
		return;
	}

	FacadeRef->IniciarModoAventura();
}

void AHUDmain::ModoMultijugador()
{
	if (!FacadeRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("No existe referencia a FacadeGameManager."));
		return;
	}

	FacadeRef->IniciarModoMultijugador();
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
		ConfigurarInputController();
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

	ModoAventura();
	DetenerMusicaInicio();
	RemoverInputController();
	MostrarOnGame();
}

void AHUDmain::MostrarPantallaCarga2()
{
	if (!PantallaCargaInstance && PantallaCargaClass)
	{
		PantallaCargaInstance = CreateWidget<UWidgetPantallaCarga>(GetWorld(), PantallaCargaClass);
	}

	if (PantallaCargaInstance && !PantallaCargaInstance->IsInViewport())
	{
		PantallaCargaInstance->AddToViewport(5);
		PantallaCargaInstance->MostrarFondoAleatorio();
		//UGameplayStatics::SetGamePaused(GetWorld(), false);
		/*	ConfigurarInputController();*/
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle_OcultarPantallaCarga,
			this,
			&AHUDmain::OcultarPantallaCarga2,
			4.9f,
			false
		);
	}
}

void AHUDmain::OcultarPantallaCarga2()
{
	if (PantallaCargaInstance && PantallaCargaInstance->IsInViewport())
	{
		PantallaCargaInstance->RemoveFromParent();
		//RemoverInputController();
	}
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

		APawn* Pawn = PC->GetPawn();
		if (Pawn)
		{
			AMobileSpacePawn* Nave = Cast<AMobileSpacePawn>(Pawn);
			if (Nave)
			{
				Nave->InicializarPowerUpsHUD();
			}
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

void AHUDmain::MostrarModoMultijugador()
{
	if (!ModoMultijugadorInstance && ModoMultijugadorClass)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		ModoMultijugadorInstance = CreateWidget<UWidget_Modo_multijugador>(PC, ModoMultijugadorClass);

		if (ModoMultijugadorInstance)
			ModoMultijugadorInstance->MostrarVentana(false);
	}

	if (ModoMultijugadorInstance && !ModoMultijugadorInstance->IsInViewport())
	{
		ModoMultijugadorInstance->AddToViewport();
	}
}


void AHUDmain::OcultarModoMultijugador()
{
	if (ModoMultijugadorInstance && ModoMultijugadorInstance->IsInViewport())
	{
		ModoMultijugadorInstance->RemoveFromParent();
		//UGameplayStatics::SetGamePaused(GetWorld(), false);
	}
}

void AHUDmain::MostrarSalaEspera()
{
	if (!WidgetSalaEsperaInstance && WidgetSalaEsperaClass)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		WidgetSalaEsperaInstance = CreateWidget<UWidgetSalaEspera>(PC, WidgetSalaEsperaClass);
	}

	if (WidgetSalaEsperaInstance && !WidgetSalaEsperaInstance->IsInViewport())
	{
		WidgetSalaEsperaInstance->AddToViewport();

		GetWorldTimerManager().SetTimer(TimerHandle_OcultarPantallaCarga, this, &AHUDmain::MostrarPantallaCargaMulti, 3.0f, false);
	}
}

void AHUDmain::OcultarSalaEspera()
{
	if (WidgetSalaEsperaInstance && WidgetSalaEsperaInstance->IsInViewport())
	{
		WidgetSalaEsperaInstance->RemoveFromParent();
	}
}

void AHUDmain::MostrarPantallaCargaMulti()
{
	if (!WidgetPCMultiInstance && WidgetPCMultiInstance)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		WidgetPCMultiInstance = CreateWidget<UWidgetPCMulti>(PC, WidgetPCMultiClass);
	}

	if (WidgetPCMultiInstance && !WidgetPCMultiInstance->IsInViewport())
	{
		WidgetPCMultiInstance->AddToViewport();

		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AHUDmain::OcultarPantallaCargaMulti, 3.0f, false);
	}
}


void AHUDmain::OcultarPantallaCargaMulti()
{
	if (WidgetPCMultiInstance && WidgetPCMultiInstance->IsInViewport())
	{
		DetenerMusicaInicio();
		RemoverInputController();
		MostrarOnGameMulti();

		WidgetPCMultiInstance->RemoveFromParent();
	}
}


void AHUDmain::MostrarOnGameMulti()
{
	if (!WidgetOnGameMultiInstance && WidgetOnGameMultiClass)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		WidgetOnGameMultiInstance = CreateWidget<UWidgetOnGameMulti>(PC, WidgetOnGameMultiClass);
	}
	if (WidgetOnGameMultiInstance && !WidgetOnGameMultiInstance->IsInViewport())
	{
		WidgetOnGameMultiInstance->AddToViewport();
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PC)
		{
			UGameplayStatics::SetGamePaused(GetWorld(), false);
			OcultarPantallaCargaMulti();

			PC->bShowMouseCursor = true;
			FInputModeGameAndUI InputMode;
			InputMode.SetWidgetToFocus(WidgetOnGameInstance->TakeWidget());
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PC->SetInputMode(InputMode);
		}
	}
}

void AHUDmain::OcultarOnGameMulti()
{
	if (WidgetOnGameMultiInstance && WidgetOnGameMultiInstance->IsInViewport())
	{
		WidgetOnGameMultiInstance->RemoveFromParent();
	}
}

void AHUDmain::MostrarGameOver()
{
	if (!WidgetGameOverInstance && WidgetGameOverClass)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		WidgetGameOverInstance = CreateWidget<UWidgetGameOver>(PC, WidgetGameOverClass);
	}
	if (WidgetGameOverInstance)
	{
		if (!WidgetGameOverInstance->IsInViewport())
			WidgetGameOverInstance->AddToViewport();
	}
}

void AHUDmain::OcultarGameOver()
{
	if (WidgetGameOverInstance && WidgetGameOverInstance->IsInViewport())
	{
		WidgetGameOverInstance->RemoveFromParent();
	}
}

void AHUDmain::MostrarNivel(const FString& Mensaje)
{
	if (!WidgetLevelInstance && WidgetLevelClass)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		WidgetLevelInstance = CreateWidget<UWidget_Indicar_level>(PC, WidgetLevelClass);
	}

	if (WidgetLevelInstance)
	{
		if (!WidgetLevelInstance->IsInViewport())
			WidgetLevelInstance->AddToViewport();

		WidgetLevelInstance->SetVisibility(ESlateVisibility::Visible);
		WidgetLevelInstance->MostrarMensaje(Mensaje);

	
	}
}

void AHUDmain::OcultarNivel()
{
	
	if (WidgetLevelInstance && WidgetLevelInstance->IsInViewport())
	{
		WidgetLevelInstance->RemoveFromParent();
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
		//UGameplayStatics::SetGamePaused(GetWorld(), true);
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
	OcultarAjustes();
	OcultarCreditos();
	OcultarModoJuego();
	OcultarPantallaCarga();
	OcultarModoJuego();
	OcultarAjustes();
	OcultarCreditos();
}
