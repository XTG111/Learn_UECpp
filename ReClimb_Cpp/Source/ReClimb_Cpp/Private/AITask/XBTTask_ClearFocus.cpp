// Fill out your copyright notice in the Description page of Project Settings.


#include "AITask/XBTTask_ClearFocus.h"
#include "AIController.h"
#include "AI/XAIController.h"
#include "AI/XAI_Character.h"

EBTNodeResult::Type UXBTTask_ClearFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//»ñÈ¡AI¿ØÖÆÆ÷
	AXAIController* AICon = Cast<AXAIController>(OwnerComp.GetAIOwner());
	if (AICon)
	{
		AXAI_Character* ControlledPawn = Cast<AXAI_Character>(AICon->GetPawn());
		AICon->ClearFocus(EAIFocusPriority::LastFocusPriority);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
