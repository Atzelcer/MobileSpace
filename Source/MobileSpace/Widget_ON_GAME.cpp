// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget_ON_GAME.h"
#include "Kismet/GameplayStatics.h"
#include "HUDmain.h"
#include "MobileSpacePawn.h"

void UWidget_ON_GAME::NativeConstruct()
{
	Super::NativeConstruct();

	bEscudoActivo = true;
	bVelocidadActiva = true;

	TexEscudo_ON = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/Game/Imagenes/botones/escudo.escudo'"));
	TexEscudo_OFF = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/Game/Imagenes/botones/escudo_off.escudo_off'"));
	TexVelocidad_ON = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/Game/Imagenes/botones/ONvelocidad.ONvelocidad'"));
	TexVelocidad_OFF = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/Game/Imagenes/botones/ONvelocidad_off.ONvelocidad_off'"));
	ONmissil = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/Game/Imagenes/botones/ONmissil.ONmissil'"));
	ONmissil_off = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/Game/Imagenes/botones/ONmissil_off.ONmissil_off'"));

	if (Button_escudo) Button_escudo->OnClicked.AddDynamic(this, &UWidget_ON_GAME::OnEscudoClicked);
	if (Button_velocidad) Button_velocidad->OnClicked.AddDynamic(this, &UWidget_ON_GAME::OnVelocidadClicked);
	if (Button_missil) Button_missil->OnClicked.AddDynamic(this, &UWidget_ON_GAME::OnMissilClicked);
	if (Button_menu) Button_menu->OnClicked.AddDynamic(this, &UWidget_ON_GAME::OnMenuClicked);

	CargarTexturasPanels();
	CambiarPanelsAleatorios();
	ActualizarEstadoBotones();
}

void UWidget_ON_GAME::CargarTexturasPanels()
{
	PanelGameTextures.Empty();
	IconHamburTextures.Empty();

	const FString RutasGameOn[4] = {
		TEXT("Texture2D'/Game/Imagenes/gameOn/version01.version01'"),
		TEXT("Texture2D'/Game/Imagenes/gameOn/version02.version02'"),
		TEXT("Texture2D'/Game/Imagenes/gameOn/version03.version03'"),
		TEXT("Texture2D'/Game/Imagenes/gameOn/version04.version04'")
	};

	const FString RutasHambur[4] = {
		TEXT("Texture2D'/Game/Imagenes/iconos/hambur1.hambur1'"),
		TEXT("Texture2D'/Game/Imagenes/iconos/hambur2.hambur2'"),
		TEXT("Texture2D'/Game/Imagenes/iconos/hambur3.hambur3'"),
		TEXT("Texture2D'/Game/Imagenes/iconos/hambur4.hambur4'")
	};

	for (int32 i = 0; i < 4; i++)
	{
		if (UTexture2D* TexPanel = LoadObject<UTexture2D>(nullptr, *RutasGameOn[i]))
			PanelGameTextures.Add(TexPanel);

		if (UTexture2D* TexHambur = LoadObject<UTexture2D>(nullptr, *RutasHambur[i]))
			IconHamburTextures.Add(TexHambur);
	}
}

void UWidget_ON_GAME::CambiarPanelsAleatorios()
{
	if (PanelGameTextures.Num() == 0 || IconHamburTextures.Num() == 0)
		return;

	int32 Index = FMath::RandRange(0, PanelGameTextures.Num() - 1);

	if (Image_panel01) Image_panel01->SetBrushFromTexture(PanelGameTextures[Index]);
	if (Image_panel02) Image_panel02->SetBrushFromTexture(PanelGameTextures[Index]);
	if (Image_panel03) Image_panel03->SetBrushFromTexture(IconHamburTextures[Index]);
}

void UWidget_ON_GAME::ActualizarVida(int32 Valor)
{
	if (TextBlock_cantidad_vida)
		TextBlock_cantidad_vida->SetText(FText::AsNumber(Valor));
}

void UWidget_ON_GAME::ActualizarVelocidad(float Velocidad)
{
	if (TextBlock_cantidad_velocidad)
		TextBlock_cantidad_velocidad->SetText(FText::AsNumber((int32)Velocidad));
	ActualizarEstadoBotones();
}

void UWidget_ON_GAME::ActualizarMisiles(int32 Cantidad)
{
	if (TextBlock_cantidad_misil)
		TextBlock_cantidad_misil->SetText(FText::AsNumber(Cantidad));
	ActualizarEstadoBotones();
}

void UWidget_ON_GAME::ActualizarEscudo(int32 Cantidad)
{
	if (TextBlock_cantidad_escudo)
		TextBlock_cantidad_escudo->SetText(FText::AsNumber(Cantidad));
	ActualizarEstadoBotones();
}

void UWidget_ON_GAME::ActualizarSobrecarga(float Valor, float MaxValor)
{
	if (ProgressBar_Sobrecarga_Disparo)
	{
		float Progreso = FMath::Clamp(Valor / MaxValor, 0.f, 1.f);
		ProgressBar_Sobrecarga_Disparo->SetPercent(Progreso);
	}
}

void UWidget_ON_GAME::CambiarArma(UTexture2D* NuevaArma)
{
	if (image_arma_cambio && NuevaArma)
		image_arma_cambio->SetBrushFromTexture(NuevaArma);
}

void UWidget_ON_GAME::ActualizarEstadoBotones()
{
	int32 Escudos = FCString::Atoi(*TextBlock_cantidad_escudo->GetText().ToString());
	int32 Velocidad = FCString::Atoi(*TextBlock_cantidad_velocidad->GetText().ToString());
	int32 Misiles = FCString::Atoi(*TextBlock_cantidad_misil->GetText().ToString());

	if (Image_ESCUDO)
		Image_ESCUDO->SetBrushFromTexture((Escudos > 0 && bEscudoActivo) ? TexEscudo_ON : TexEscudo_OFF);

	if (Image_VELOCIDAD)
		Image_VELOCIDAD->SetBrushFromTexture((Velocidad > 0 && bVelocidadActiva) ? TexVelocidad_ON : TexVelocidad_OFF);

	if (Image_Missil)
		Image_Missil->SetBrushFromTexture((Misiles > 0) ? ONmissil : ONmissil_off);
}

void UWidget_ON_GAME::OnEscudoClicked()
{
	if (!bEscudoActivo) return;

	int32 Cantidad = FCString::Atoi(*TextBlock_cantidad_escudo->GetText().ToString());
	if (Cantidad <= 0)
	{
		bEscudoActivo = false;
	}
	else
	{
		TextBlock_cantidad_escudo->SetText(FText::AsNumber(Cantidad - 1));
		bEscudoActivo = false;
	}
	ActualizarEstadoBotones();
}

void UWidget_ON_GAME::OnVelocidadClicked()
{
	if (!bVelocidadActiva) return;

	int32 Cantidad = FCString::Atoi(*TextBlock_cantidad_velocidad->GetText().ToString());
	if (Cantidad <= 0)
	{
		bVelocidadActiva = false;
	}
	else
	{
		TextBlock_cantidad_velocidad->SetText(FText::AsNumber(Cantidad - 1));
		bVelocidadActiva = false; 
	}
	ActualizarEstadoBotones();
}

void UWidget_ON_GAME::ReactivarEscudo()
{
	bEscudoActivo = true;
	ActualizarEstadoBotones();
}

void UWidget_ON_GAME::ReactivarVelocidad()
{
	bVelocidadActiva = true;
	ActualizarEstadoBotones();
}

void UWidget_ON_GAME::OnMissilClicked()
{
	int32 Cantidad = FCString::Atoi(*TextBlock_cantidad_misil->GetText().ToString());

	if (Cantidad <= 0)
	{
		ActualizarEstadoBotones();
		return;
	}

	TextBlock_cantidad_misil->SetText(FText::AsNumber(Cantidad - 1));
	ActualizarEstadoBotones();


	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC)
	{
		return;
	}

	APawn* Pawn = PC->GetPawn();
	if (!Pawn)
	{
		return;
	}

	AMobileSpacePawn* Nave = Cast<AMobileSpacePawn>(Pawn);
	if (Nave)
	{
		Nave->DispararMisil();
	}

}


void UWidget_ON_GAME::OnMenuClicked()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC) return;

	AHUDmain* HUD = Cast<AHUDmain>(PC->GetHUD());
	if (HUD)
	{
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		HUD->MostrarPause();
	}
}
