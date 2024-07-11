// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "XBTService_UpdateDisToTarget.generated.h"

/**
 *
 */
UCLASS()
class RECLIMB_CPP_API UXBTService_UpdateDisToTarget : public UBTService
{
	GENERATED_BODY()
public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FBlackboardKeySelector AttackTarget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FBlackboardKeySelector DistanceToTarget;

};
