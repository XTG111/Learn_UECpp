// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/XAI_Character.h"
#include "XAI_GunCharacter.generated.h"

/**
 *
 */
UCLASS()
class RECLIMB_CPP_API AXAI_GunCharacter : public AXAI_Character
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	void EquipWeapon_Implementation() override;
	void UnEquipWeapon_Implementation() override;
	float SetMovementSpeed_Implementation(EAIMovement SpeedEnum) override;
	void GetIdealRange_Implementation(float& AttackRadius, float& DefendRadius) override;
};
