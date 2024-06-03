// Fill out your copyright notice in the Description page of Project Settings.


#include "AITask/XBTTask_SheathSword.h"
#include "AI/XAIController.h"
#include "AI/XAI_Character.h"

EBTNodeResult::Type UXBTTask_SheathSword::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AXAIController* AICon = Cast<AXAIController>(OwnerComp.GetAIOwner());
	if (AICon)
	{
		AXAI_Character* ControlledPawn = Cast<AXAI_Character>(AICon->GetPawn());
		FinishDelegate.BindUObject(this, &UXBTTask_SheathSword::FinishLast, &OwnerComp);
		ControlledPawn->CallOnSheathSword.Clear();
		ControlledPawn->CallOnSheathSword.AddDynamic(this, &UXBTTask_SheathSword::Finish);
		ControlledPawn->SheathSword();
	}
	return EBTNodeResult::InProgress;
}

void UXBTTask_SheathSword::Finish()
{
	FinishDelegate.ExecuteIfBound();
}

void UXBTTask_SheathSword::FinishLast(UBehaviorTreeComponent* OwnerComp)
{
	FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
}
