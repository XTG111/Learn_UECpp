// Fill out your copyright notice in the Description page of Project Settings.


#include "AITask/XBTTask_EquipWeapon.h"
#include "AI/XAIController.h"
#include "AI/XAI_Character.h"

EBTNodeResult::Type UXBTTask_EquipWeapon::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AXAIController* AICon = Cast<AXAIController>(OwnerComp.GetAIOwner());
	if (AICon)
	{
		AXAI_Character* ControlledPawn = Cast<AXAI_Character>(AICon->GetPawn());
		FinishDelegate.BindUObject(this, &UXBTTask_EquipWeapon::FinishLast, &OwnerComp);
		ControlledPawn->CallOnEquipWeapon.Clear();
		ControlledPawn->CallOnEquipWeapon.AddDynamic(this, &UXBTTask_EquipWeapon::Finish);
		if (ControlledPawn && ControlledPawn->Implements<UXAIInterface>())
		{
			IXAIInterface::Execute_EquipWeapon(ControlledPawn);
		}
	}
	return EBTNodeResult::InProgress;
}

void UXBTTask_EquipWeapon::Finish()
{
	FinishDelegate.ExecuteIfBound();
}

void UXBTTask_EquipWeapon::FinishLast(UBehaviorTreeComponent* OwnerComp)
{
	FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
}


