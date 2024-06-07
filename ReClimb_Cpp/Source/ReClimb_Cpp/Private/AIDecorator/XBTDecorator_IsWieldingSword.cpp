// Fill out your copyright notice in the Description page of Project Settings.


#include "AIDecorator/XBTDecorator_IsWieldingSword.h"
#include "AI/XAIController.h"
#include "AI/XAI_Character.h"

bool UXBTDecorator_IsWieldingSword::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AXAIController* AICon = Cast<AXAIController>(OwnerComp.GetAIOwner());
	if (AICon)
	{
		AXAI_Character* ControlledPawn = Cast<AXAI_Character>(AICon->GetPawn());
		if (ControlledPawn->GetIsWiledWeapon())
		{
			return true;
		}
	}
	return false;
}
