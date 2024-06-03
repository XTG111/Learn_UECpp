// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "XBTDecorator_IsWieldingSword.generated.h"

/**
 *
 */
UCLASS()
class RECLIMB_CPP_API UXBTDecorator_IsWieldingSword : public UBTDecorator
{
	GENERATED_BODY()
public:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};
