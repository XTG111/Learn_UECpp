// Fill out your copyright notice in the Description page of Project Settings.


#include "AITask/XBTTask_MageTeleport.h"
#include "AI/XAIController.h"
#include "AI/XAI_MageCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UXBTTask_MageTeleport::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AICon = Cast<AXAIController>(OwnerComp.GetAIOwner());
	FinishSuccessDelegate.BindUObject(this, &UXBTTask_MageTeleport::FinishSuccessEnd, &OwnerComp);

	if (AICon)
	{
		ControlledPawn = Cast<AXAI_MageCharacter>(AICon->GetPawn());
		FVector Loc = AICon->GetBlackboardComponent()->GetValueAsVector(TeleportLocation.SelectedKeyName);
		ControlledPawn->OnTeleportEnd.AddDynamic(this, &UXBTTask_MageTeleport::TeleportEnd);
		ControlledPawn->Teleport(Loc);
	}
	else
	{
		return EBTNodeResult::Failed;
	}
	return EBTNodeResult::InProgress;
}

void UXBTTask_MageTeleport::TeleportEnd()
{
	ControlledPawn->OnTeleportEnd.Clear();
	FinishSuccessDelegate.ExecuteIfBound();
}

void UXBTTask_MageTeleport::FinishSuccessEnd(UBehaviorTreeComponent* OwnerComp)
{
	FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
}
