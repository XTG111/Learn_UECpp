// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "XBTTask_SeekingFocus.generated.h"

/**
 *
 */
UCLASS()
class RECLIMB_CPP_API UXBTTask_SeekingFocus : public UBTTask_BlueprintBase
{
	GENERATED_BODY()
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FBlackboardKeySelector TargetPoint;
};
