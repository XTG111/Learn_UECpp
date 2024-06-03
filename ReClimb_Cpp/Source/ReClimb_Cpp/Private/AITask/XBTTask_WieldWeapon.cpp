// Fill out your copyright notice in the Description page of Project Settings.


#include "AITask/XBTTask_WieldWeapon.h"
#include "AI/XAIController.h"
#include "AI/XAI_Character.h"

EBTNodeResult::Type UXBTTask_WieldWeapon::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AXAIController* AICon = Cast<AXAIController>(OwnerComp.GetAIOwner());
	if (AICon)
	{
		AXAI_Character* ControlledPawn = Cast<AXAI_Character>(AICon->GetPawn());
		FinishDelegate.BindUObject(this, &UXBTTask_WieldWeapon::FinishLast, &OwnerComp);
		ControlledPawn->CallOnWieldSword.Clear();
		ControlledPawn->CallOnWieldSword.AddDynamic(this, &UXBTTask_WieldWeapon::Finish);
		ControlledPawn->WireldSword();
	}
	return EBTNodeResult::InProgress;
}

void UXBTTask_WieldWeapon::Finish()
{
	FinishDelegate.ExecuteIfBound();
}

void UXBTTask_WieldWeapon::FinishLast(UBehaviorTreeComponent* OwnerComp)
{
	FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
}

