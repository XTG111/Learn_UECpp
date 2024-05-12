// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "StopMontageNotify.generated.h"

/**
 * 
 */
UCLASS()
class RECLIMB_CPP_API UStopMontageNotify : public UAnimNotify
{
	GENERATED_BODY()
public:
	bool Received_Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) const;
};
