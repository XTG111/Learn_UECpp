// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "XBTTask_FocusTarget.generated.h"

/**
 *
 */
UCLASS()
class RECLIMB_CPP_API UXBTTask_FocusTarget : public UBTTask_BlueprintBase
{
	GENERATED_BODY()
public:
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		//TSubclassOf<FBlackboardKeySelector> AttackTargetKey;
		FBlackboardKeySelector AttackTargetKey;
};
