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
	bInitialized = false;
}

void UCreditosC::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!V_Creditos_A)
		return;

	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(V_Creditos_A->Slot))
	{
		if (!bInitialized)
		{
			StartY = CanvasSlot->GetPosition().Y;

			float TextHeight = V_Creditos_A->GetDesiredSize().Y;
			float ScreenHeight = MyGeometry.GetLocalSize().Y;

			ResetOffset = TextHeight + ScreenHeight;

			bInitialized = true;
		}

		FVector2D Pos = CanvasSlot->GetPosition();
		Pos.Y -= ScrollSpeed * InDeltaTime;
		CanvasSlot->SetPosition(Pos);

		if (Pos.Y <= -ResetOffset)
		{
			Pos.Y = StartY;
			CanvasSlot->SetPosition(Pos);
		}
	}
}

void UCreditosC::OnVolverClicked()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PlayerController) return;

	AHUDmain* HUD = Cast<AHUDmain>(PlayerController->GetHUD());
	if (HUD)
	{
		RemoveFromParent();
		HUD->MostrarPanelPrincipal();
	}
}
