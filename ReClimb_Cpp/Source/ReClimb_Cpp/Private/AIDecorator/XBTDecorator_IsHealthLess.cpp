// Fill out your copyright notice in the Description page of Project Settings.


#include "AIDecorator/XBTDecorator_IsHealthLess.h"
#include "AI/XAIController.h"
#include "AI/XAI_Character.h"
#include "BehaviorTree/BlackboardComponent.h"

bool UXBTDecorator_IsHealthLess::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AXAIController* AICon = Cast<AXAIController>(OwnerComp.GetAIOwner());
	if (AICon)
	{
		AXAI_Character* ControlledPawn = Cast<AXAI_Character>(AICon->GetPawn());
		if (ControlledPawn && ControlledPawn->Implements<UXAIInterface>())
		{
			float curhp = IXAIInterface::Execute_GetCurrentHealth(ControlledPawn);
			float maxhp = IXAIInterface::Execute_GetMaxHealth(ControlledPawn);

			if (curhp / maxhp <= Limit) return true;
		}
	}
	return false;
}
