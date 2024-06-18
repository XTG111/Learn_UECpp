// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "Navigation/PathFollowingComponent.h"
#include "XBTTask_SwordAttack.generated.h"

/**
 *
 */
UCLASS()
class RECLIMB_CPP_API UXBTTask_SwordAttack : public UBTTask_BlueprintBase
{
	GENERATED_BODY()

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FBlackboardKeySelector AttackTargetKey;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FBlackboardKeySelector AttackRadiusKey;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int TokenNeeded;

	UPROPERTY()
		AActor* AttackActor;
	UPROPERTY()
		class AXAI_Character* ControlledPawn;
	UPROPERTY()
		class AXAIController* AICon;

	UFUNCTION()
		void MoveEndCall(FAIRequestID RequestID, EPathFollowingResult::Type Result);

	FSimpleDelegate FinishSuccessDelegate;
	FSimpleDelegate FinishFailedDelegate;


	UFUNCTION()
		void FinishAttack();
	UFUNCTION()
		void FinishSuccessEnd(UBehaviorTreeComponent* OwnerComp);
	UFUNCTION()
		void FinishFailedEnd(UBehaviorTreeComponent* OwnerComp);

};
