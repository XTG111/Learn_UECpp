// Fill out your copyright notice in the Description page of Project Settings.


#include "AITask/XBTTask_AIHeal.h"
#include "AI/XAIController.h"
#include "AI/XAI_Character.h"

EBTNodeResult::Type UXBTTask_AIHeal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AXAIController* AICon = Cast<AXAIController>(OwnerComp.GetAIOwner());
	if (AICon)
	{
		AXAI_Character* ControlledPawn = Cast<AXAI_Character>(AICon->GetPawn());
		if (ControlledPawn && ControlledPawn->Implements<UXDamageInterface>())
		{
			float MaxHP = IXDamageInterface::Execute_GetMaxHealth(ControlledPawn);

			IXDamageInterface::Execute_Heal(ControlledPawn, 0.8f * MaxHP);
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
