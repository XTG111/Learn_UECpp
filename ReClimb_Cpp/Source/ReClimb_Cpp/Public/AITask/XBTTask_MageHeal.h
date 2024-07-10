// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "XBTTask_MageHeal.generated.h"

/**
 *
 */
UCLASS()
class RECLIMB_CPP_API UXBTTask_MageHeal : public UBTTask_BlueprintBase
{
	GENERATED_BODY()
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY()
		class AXAI_MageCharacter* ControlledPawn;
	UPROPERTY()
		class AXAIController* AICon;


	FSimpleDelegate FinishSuccessDelegate;
	UFUNCTION()
		void HealEnd();
	UFUNCTION()
		void FinishSuccessEnd(UBehaviorTreeComponent* OwnerComp);
};
