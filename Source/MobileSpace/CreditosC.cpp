// Fill out your copyright notice in the Description page of Project Settings.


#include "CreditosC.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/GameplayStatics.h"
#include "HUDmain.h"

void UCreditosC::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_volver)
		Button_volver->OnClicked.AddDynamic(this, &UCreditosC::OnVolverClicked);

	ScrollSpeed = 80.0f;

	// Guardar posición inicial del texto
	if (V_Creditos_A)
	{
		if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(V_Creditos_A->Slot))
		{
			StartY = CanvasSlot->GetPosition().Y;
		}
	}
}

void UCreditosC::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (V_Creditos_A)
	{
		if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(V_Creditos_A->Slot))
		{
			FVector2D Pos = CanvasSlot->GetPosition();
			Pos.Y -= ScrollSpeed * InDeltaTime; // mueve hacia arriba
			CanvasSlot->SetPosition(Pos);

			// Reiniciar si sale completamente de pantalla (bucle)
			if (Pos.Y < -800.0f) // ajusta según el alto del texto
			{
				Pos.Y = StartY;
				CanvasSlot->SetPosition(Pos);
			}
		}
	}
}

void UCreditosC::OnVolverClicked()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		AHUDmain* HUD = Cast<AHUDmain>(PlayerController->GetHUD());
		if (HUD)
		{
			RemoveFromParent();
			HUD->MostrarPanelPrincipal();
		}
	}
}
