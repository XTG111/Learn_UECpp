// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "XHUD.generated.h"

/**
 *
 */

 //�洢׼�ǵĽṹ��
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

	//׼�ǵ���չ
	float CrosshairSpread;
	//׼�ǵ���ɫ
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

	//����׼�ǣ�Spread���ڿ���׼�ǵı仯
	void DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread, FLinearColor DrawCrosshairColor);

public:
	void SetHUDPackage(const FHUDPackage& Package);
};
