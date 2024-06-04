// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "Navigation/PathFollowingComponent.h"
#include "XBTTask_SelfMoveTo.generated.h"

struct FPathFollowingResult;
struct FAIRequestID;
/**
 *
 */
UCLASS()
class RECLIMB_CPP_API UXBTTask_SelfMoveTo : public UBTTask_BlueprintBase
{
	GENERATED_BODY()
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FBlackboardKeySelector IdealRange;

	FSimpleDelegate FinishDelegate;

	UFUNCTION()
		void MoveEndCall(FAIRequestID RequestID, EPathFollowingResult::Type Result);
	//void MoveEndCall(FAIRequestID RequestID, const FPathFollowingResult& Result);

	UFUNCTION()
		void FinishEnd(UBehaviorTreeComponent* OwnerComp);
};
