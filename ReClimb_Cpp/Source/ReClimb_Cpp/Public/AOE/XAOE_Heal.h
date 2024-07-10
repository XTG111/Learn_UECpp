// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AOE/XAOE_OverTimeBase.h"
#include "XAOE_Heal.generated.h"

/**
 *
 */
UCLASS()
class RECLIMB_CPP_API AXAOE_Heal : public AXAOE_OverTimeBase
{
	GENERATED_BODY()
public:
	virtual void Trigger() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UParticleSystem* AttachParticle;
	UPROPERTY()
		class UParticleSystemComponent* HealEffect;

};
