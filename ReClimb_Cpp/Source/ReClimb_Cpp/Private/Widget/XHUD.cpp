// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/XHUD.h"
#include "GameFramework/PlayerController.h"
#include "Components/HorizontalBox.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Widget/XWidget_CharacterOverShow.h"

void AXHUD::BeginPlay()
{
	Super::BeginPlay();
}

void AXHUD::DrawHUD()
{
	Super::DrawHUD();

	FVector2D ViewPortSize;
	if (GEngine)
	{
		//»ñÈ¡ÆÁÄ»³ß´ç
		GEngine->GameViewport->GetViewportSize(ViewPortSize);
		const FVector2D ViewPortCenter(ViewPortSize.X / 2.f, ViewPortSize.Y / 2.f);

		if (HUDPackage.CrosshairsCenter)
		{
			//UE_LOG(LogTemp, Warning, TEXT("CrosshairsCenter"));
			FVector2D Spread(0.f, 0.f);
			DrawCrosshair(HUDPackage.CrosshairsCenter, ViewPortCenter, Spread, HUDPackage.CrosshairColor);
		}
	}
}

void AXHUD::DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread, FLinearColor DrawCrosshairColor)
{
	const float TextureWidth = Texture->GetSizeX();
	const float TextureHeight = Texture->GetSizeY();

	const FVector2D TextureDrawPoint(ViewportCenter.X - (TextureWidth / 2.f) + Spread.X, ViewportCenter.Y - (TextureHeight / 2.f) + Spread.Y);

	DrawTexture(Texture, TextureDrawPoint.X, TextureDrawPoint.Y, TextureWidth, TextureHeight, 0.f, 0.f, 1.f, 1.f, DrawCrosshairColor);
}

void AXHUD::SetHUDPackage(const FHUDPackage& Package)
{
	HUDPackage = Package;
}

void AXHUD::AddCharacterShow()
{
	PlayerController = PlayerController == nullptr ? GetOwningPlayerController() : PlayerController;
	if (PlayerController && CharacterShowWdgClass)
	{
		CharacterShowWdg = CreateWidget<UXWidget_CharacterOverShow>(PlayerController, CharacterShowWdgClass);
		CharacterShowWdg->AddToViewport();
	}
}
