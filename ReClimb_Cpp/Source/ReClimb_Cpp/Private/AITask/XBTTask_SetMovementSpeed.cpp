// Fill out your copyright notice in the Description page of Project Settings.


#include "AITask/XBTTask_SetMovementSpeed.h"
#include "AI/XAIController.h"
#include "AI/XAI_Character.h"

EBTNodeResult::Type UXBTTask_SetMovementSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AXAIController* AICon = Cast<AXAIController>(OwnerComp.GetAIOwner());
	if (AICon)
	{
		AXAI_Character* ControlledPawn = Cast<AXAI_Character>(AICon->GetPawn());
		if (ControlledPawn && ControlledPawn->Implements<UXAIInterface>())
		{
			IXAIInterface::Execute_SetMovementSpeed(ControlledPawn, SpeedEnum);
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
