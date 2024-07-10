// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AOE/XAOEBase.h"
#include "XAOE_OverTimeBase.generated.h"

/**
 *
 */
UCLASS()
class RECLIMB_CPP_API AXAOE_OverTimeBase : public AXAOEBase
{
	GENERATED_BODY()
public:
	float Duration = 5.0f;
	float SpawnInterval = 0.5f;

	virtual void Trigger() override;

	//Loop Timer
	FTimerHandle SpawnLoopTimer;
	FTimerHandle DestroyTimer;
	UFUNCTION()
		void SpawnEmitter();
	UFUNCTION()
		void DestroyEmitter();
};
