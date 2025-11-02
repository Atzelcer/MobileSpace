// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_Indicar_level.h"
#include "Kismet/KismetMathLibrary.h"
#include "Materials/MaterialInterface.h"

void UWidget_Indicar_level::NativeConstruct()
{
	Super::NativeConstruct();

	BackgroundMaterials.Add(LoadObject<UMaterialInterface>(nullptr, TEXT("MaterialInstanceConstant'/Game/INTERFAZ/MP_GREEN_Inst.MP_GREEN_Inst'")));
	BackgroundMaterials.Add(LoadObject<UMaterialInterface>(nullptr, TEXT("MaterialInstanceConstant'/Game/INTERFAZ/MP_PURPLE_Inst.MP_PURPLE_Inst'")));
	BackgroundMaterials.Add(LoadObject<UMaterialInterface>(nullptr, TEXT("MaterialInstanceConstant'/Game/INTERFAZ/MP_RED_Inst.MP_RED_Inst'")));
	BackgroundMaterials.Add(LoadObject<UMaterialInterface>(nullptr, TEXT("MaterialInstanceConstant'/Game/INTERFAZ/MP_SKY_BLUE_Inst.MP_SKY_BLUE_Inst'")));
	BackgroundMaterials.Add(LoadObject<UMaterialInterface>(nullptr, TEXT("MaterialInstanceConstant'/Game/INTERFAZ/MP_YELLOW_Inst.MP_YELLOW_Inst'")));

	CambiarColorAleatorio();
}

void UWidget_Indicar_level::CambiarColorAleatorio()
{
	if (!Colors_Change || BackgroundMaterials.Num() == 0)
		return;

	int32 Index = UKismetMathLibrary::RandomIntegerInRange(0, BackgroundMaterials.Num() - 1);
	UMaterialInterface* RandomMat = BackgroundMaterials[Index];

	Colors_Change->SetBrushFromMaterial(RandomMat);
}

void UWidget_Indicar_level::MostrarMensaje(const FString& Mensaje)
{
	if (Mensaje_Pantalla_Level)
		Mensaje_Pantalla_Level->SetText(FText::FromString(Mensaje));

	CambiarColorAleatorio();
}
