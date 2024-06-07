// Fill out your copyright notice in the Description page of Project Settings.


#include "AITask/XBTTask_UnEquipWeapon.h"
#include "AI/XAIController.h"
#include "AI/XAI_Character.h"

EBTNodeResult::Type UXBTTask_UnEquipWeapon::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AXAIController* AICon = Cast<AXAIController>(OwnerComp.GetAIOwner());
	if (AICon)
	{
		AXAI_Character* ControlledPawn = Cast<AXAI_Character>(AICon->GetPawn());
		FinishDelegate.BindUObject(this, &UXBTTask_UnEquipWeapon::FinishLast, &OwnerComp);
		ControlledPawn->CallOnUnEquipWeapon.Clear();
		ControlledPawn->CallOnUnEquipWeapon.AddDynamic(this, &UXBTTask_UnEquipWeapon::Finish);
		if (ControlledPawn && ControlledPawn->Implements<UXAIInterface>())
		{
			IXAIInterface::Execute_UnEquipWeapon(ControlledPawn);
		}
	}
	return EBTNodeResult::InProgress;
}

void UXBTTask_UnEquipWeapon::Finish()
{
	FinishDelegate.ExecuteIfBound();
}

void UXBTTask_UnEquipWeapon::FinishLast(UBehaviorTreeComponent* OwnerComp)
{
	FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
}

