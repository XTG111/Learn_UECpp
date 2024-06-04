// Fill out your copyright notice in the Description page of Project Settings.


#include "AIDecorator/XBTDecorator_CheckInDefendRange.h"
#include "AI/XAIController.h"
#include "AI/XAI_Character.h"
#include "BehaviorTree/BlackboardComponent.h"

bool UXBTDecorator_CheckInDefendRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AXAIController* AICon = Cast<AXAIController>(OwnerComp.GetAIOwner());
	if (AICon)
	{
		AXAI_Character* ControlledPawn = Cast<AXAI_Character>(AICon->GetPawn());
		float check = AICon->GetBlackboardComponent()->GetValueAsFloat(IdealRange.SelectedKeyName);

		float DisToTarget = ControlledPawn->GetDistanceTo(AICon->GetAttackTargetActor()) - 50.0f;

		if (DisToTarget <= check)
		{
			return true;
		}
	}
	return false;
}
