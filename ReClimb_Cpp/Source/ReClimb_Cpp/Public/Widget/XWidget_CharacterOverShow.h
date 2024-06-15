// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "XWidget_CharacterOverShow.generated.h"

/**
 *
 */
UCLASS()
class RECLIMB_CPP_API UXWidget_CharacterOverShow : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* HealthBar;

};
