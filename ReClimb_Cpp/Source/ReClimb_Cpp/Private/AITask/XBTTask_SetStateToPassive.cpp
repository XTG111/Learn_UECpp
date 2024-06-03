// Fill out your copyright notice in the Description page of Project Settings.


#include "AITask/XBTTask_SetStateToPassive.h"
#include "AI/XAIController.h"
#include "AI/XAI_Character.h"

EBTNodeResult::Type UXBTTask_SetStateToPassive::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AXAIController* AICon = Cast<AXAIController>(OwnerComp.GetAIOwner());
	if (AICon)
	{
		AICon->SetStateAsPassive();
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
