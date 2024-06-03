// Fill out your copyright notice in the Description page of Project Settings.


#include "AIDecorator/XBTDecorator_PatrolRoute.h"
#include "AI/XAIController.h"
#include "AI/XAI_Character.h"
#include "Other/XLineBase.h"

bool UXBTDecorator_PatrolRoute::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AXAIController* AICon = Cast<AXAIController>(OwnerComp.GetAIOwner());
	if (AICon)
	{
		AXAI_Character* ControlledPawn = Cast<AXAI_Character>(AICon->GetPawn());
		if (ControlledPawn && ControlledPawn->Implements<UXAIInterface>())
		{
			if (IXAIInterface::Execute_GetPatrolRoute(ControlledPawn))
			{
				return true;
			}
		}
	}
	return false;
}
