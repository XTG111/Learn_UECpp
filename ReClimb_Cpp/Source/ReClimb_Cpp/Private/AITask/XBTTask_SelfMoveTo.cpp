// Fill out your copyright notice in the Description page of Project Settings.


#include "AITask/XBTTask_SelfMoveTo.h"
#include "AI/XAIController.h"
#include "AI/XAI_Character.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UXBTTask_SelfMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AXAIController* AICon = Cast<AXAIController>(OwnerComp.GetAIOwner());
	if (AICon)
	{
		AXAI_Character* ControlledPawn = Cast<AXAI_Character>(AICon->GetPawn());
		if (ControlledPawn)
		{
			//AICon->GetPathFollowingComponent()->OnRequestFinished.Clear();
			AICon->ReceiveMoveCompleted.Clear();
			FinishDelegate.BindUObject(this, &UXBTTask_SelfMoveTo::FinishEnd, &OwnerComp);
			AICon->ReceiveMoveCompleted.AddDynamic(this, &UXBTTask_SelfMoveTo::MoveEndCall);
			AActor* AttackActor = AICon->GetAttackTargetActor();
			//通过设定黑板值，限制范围
			float Radius = AICon->GetBlackboardComponent()->GetValueAsFloat(IdealRange.SelectedKeyName);
			AICon->MoveToActor(AttackActor, Radius, false);
			//AICon->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &UXBTTask_SelfMoveTo::MoveEndCall);
		}
	}
	return EBTNodeResult::InProgress;
}

void UXBTTask_SelfMoveTo::MoveEndCall(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	FinishDelegate.ExecuteIfBound();
}

void UXBTTask_SelfMoveTo::FinishEnd(UBehaviorTreeComponent* OwnerComp)
{
	FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
}
