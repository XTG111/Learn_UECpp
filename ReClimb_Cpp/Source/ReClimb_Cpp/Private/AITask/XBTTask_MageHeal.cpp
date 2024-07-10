// Fill out your copyright notice in the Description page of Project Settings.


#include "AITask/XBTTask_MageHeal.h"
#include "AI/XAIController.h"
#include "AI/XAI_MageCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UXBTTask_MageHeal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AICon = Cast<AXAIController>(OwnerComp.GetAIOwner());
	FinishSuccessDelegate.BindUObject(this, &UXBTTask_MageHeal::FinishSuccessEnd, &OwnerComp);

	if (AICon)
	{
		ControlledPawn = Cast<AXAI_MageCharacter>(AICon->GetPawn());
		ControlledPawn->OnHealOverTimeEnd.Clear();
		ControlledPawn->OnHealOverTimeEnd.AddDynamic(this, &UXBTTask_MageHeal::HealEnd);
		ControlledPawn->HealOverTime();
	}
	else
	{
		return EBTNodeResult::Failed;
	}
	return EBTNodeResult::InProgress;
}

void UXBTTask_MageHeal::HealEnd()
{
	FinishSuccessDelegate.ExecuteIfBound();
}

void UXBTTask_MageHeal::FinishSuccessEnd(UBehaviorTreeComponent* OwnerComp)
{
	FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
}
