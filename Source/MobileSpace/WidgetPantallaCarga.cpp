// Fill out your copyright notice in the Description page of Project Settings.

#include "WidgetPantallaCarga.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "TimerManager.h"

void UWidgetPantallaCarga::NativeConstruct()
{
	Super::NativeConstruct();

	// Cargar texturas (seguro en tiempo de ejecución)
	FondosCarga.Empty();

	const TArray<FString> RutasTexturas = {
		TEXT("Texture2D'/Game/Imagenes/pantallaCarga/PC1_0.PC1_0'"),
		TEXT("Texture2D'/Game/Imagenes/pantallaCarga/PC2_0.PC2_0'"),
		TEXT("Texture2D'/Game/Imagenes/pantallaCarga/PC3_0.PC3_0'"),
		TEXT("Texture2D'/Game/Imagenes/pantallaCarga/PC4_0.PC4_0'"),
		TEXT("Texture2D'/Game/Imagenes/pantallaCarga/PC5_0_.PC5_0_'"),
		TEXT("Texture2D'/Game/Imagenes/pantallaCarga/PC6_0.PC6_0'"),
		TEXT("Texture2D'/Game/Imagenes/pantallaCarga/PC7_0.PC7_0'"),
		TEXT("Texture2D'/Game/Imagenes/pantallaCarga/PC8_0.PC8_0'"),
		TEXT("Texture2D'/Game/Imagenes/pantallaCarga/PC9_0.PC9_0'"),
		TEXT("Texture2D'/Game/Imagenes/pantallaCarga/PC10_0.PC10_0'"),
		TEXT("Texture2D'/Game/Imagenes/pantallaCarga/PC11_0.PC11_0'"),
		TEXT("Texture2D'/Game/Imagenes/pantallaCarga/PC12_0.PC12_0'")
	};

	for (const FString& Ruta : RutasTexturas)
	{
		if (UTexture2D* Tex = LoadObject<UTexture2D>(nullptr, *Ruta))
			FondosCarga.Add(Tex);
		//else if (GEngine)
		//	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("No se encontró: %s"), *Ruta));
	}

	//if (GEngine)
	//{
	//	FString Msg = FString::Printf(TEXT("WidgetPantallaCarga: %d texturas cargadas"), FondosCarga.Num());
	//	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, Msg);
	//}

	// Mostrar un fondo inicial
	MostrarFondoAleatorio();

	//// Ocultar después de 5 segundos
	//GetWorld()->GetTimerManager().SetTimer(
	//	TimerHandle_Ocultar,
	//	this,
	//	&UWidgetPantallaCarga::OcultarDespuesDeDelay,
	//	5.0f,
	//	false
	//);
}

void UWidgetPantallaCarga::MostrarFondoAleatorio()
{
	bool bDatosValidos = true;

	if (!Image_carga_level)
	{
		bDatosValidos = false;
		//if (GEngine)
		//	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("WidgetPantallaCarga: Image_carga_level es NULL"));
	}

	if (FondosCarga.Num() == 0)
	{
		bDatosValidos = false;
		/*if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("WidgetPantallaCarga: No hay texturas cargadas"));*/
	}

	//if (!bDatosValidos)
	//{
	//	if (GEngine)
	//		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("WidgetPantallaCarga: No se puede mostrar fondo (datos inválidos)"));
	//	return;
	//}

	int32 IndexAleatorio = FMath::RandRange(0, FondosCarga.Num() - 1);
	UTexture2D* FondoElegido = FondosCarga[IndexAleatorio];

	if (FondoElegido)
	{
		FSlateBrush Brush;
		Brush.SetResourceObject(FondoElegido);
		Brush.ImageSize = FVector2D(1920.f, 1080.f);
		Image_carga_level->SetBrush(Brush);

		//if (GEngine)
		//{
		//	FString Msg = FString::Printf(TEXT("WidgetPantallaCarga: Fondo aplicado (índice %d)"), IndexAleatorio);
		//	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, Msg);
		//}
	}
	else
	{
		//if (GEngine)
		//{
		//	FString MsgError = FString::Printf(TEXT("WidgetPantallaCarga: Fondo NULL (índice %d)"), IndexAleatorio);
		//	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, MsgError);
		//}
	}
}

void UWidgetPantallaCarga::OcultarDespuesDeDelay()
{
	RemoveFromParent();

	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, TEXT("WidgetPantallaCarga: Ocultado tras 5 segundos"));
}
