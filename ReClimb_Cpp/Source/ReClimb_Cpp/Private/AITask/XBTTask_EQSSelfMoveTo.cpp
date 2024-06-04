// Fill out your copyright notice in the Description page of Project Settings.


#include "AITask/XBTTask_EQSSelfMoveTo.h"
#include "AI/XAIController.h"
#include "AI/XAI_Character.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UXBTTask_EQSSelfMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AXAIController* AICon = Cast<AXAIController>(OwnerComp.GetAIOwner());
	if (AICon)
	{
		AXAI_Character* ControlledPawn = Cast<AXAI_Character>(AICon->GetPawn());
		if (ControlledPawn)
		{
			//AICon->GetPathFollowingComponent()->OnRequestFinished.Clear();
			AICon->ReceiveMoveCompleted.Clear();
			FinishDelegate.BindUObject(this, &UXBTTask_EQSSelfMoveTo::FinishEnd, &OwnerComp);
			AICon->ReceiveMoveCompleted.AddDynamic(this, &UXBTTask_EQSSelfMoveTo::MoveEndCall);
			AActor* AttackActor = AICon->GetAttackTargetActor();
			//通过设定黑板值，限制范围
			FVector Loc = AICon->GetBlackboardComponent()->GetValueAsVector(Location.SelectedKeyName);
			AICon->MoveToLocation(Loc, 10.0f, false);

			//AICon->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &UXBTTask_EQSSelfMoveTo::MoveEndCall);
		}
	}
	return EBTNodeResult::InProgress;
}

void UXBTTask_EQSSelfMoveTo::MoveEndCall(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	FinishDelegate.ExecuteIfBound();
}

void UXBTTask_EQSSelfMoveTo::FinishEnd(UBehaviorTreeComponent* OwnerComp)
{
	FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
}

