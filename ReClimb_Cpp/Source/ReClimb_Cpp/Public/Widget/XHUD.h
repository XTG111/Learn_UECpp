// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "XHUD.generated.h"

/**
 *
 */

 //存储准星的结构体
USTRUCT(BlueprintType)
struct FHUDPackage
{
	GENERATED_BODY()
public:

	class UTexture2D* CrosshairsCenter;
	UTexture2D* CrosshairsLeft;
	UTexture2D* CrosshairsRight;
	UTexture2D* CrosshairsTop;
	UTexture2D* CrosshairsBottom;

	//准星的扩展
	float CrosshairSpread;
	//准星的颜色
	FLinearColor CrosshairColor;
};

UCLASS()
class RECLIMB_CPP_API AXHUD : public AHUD
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
public:
	FHUDPackage HUDPackage;

	virtual void DrawHUD() override;

	UPROPERTY()
		class APlayerController* PlayerController;

	//绘制准星，Spread用于控制准星的变化
	void DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread, FLinearColor DrawCrosshairColor);

public:
	void SetHUDPackage(const FHUDPackage& Package);
};
