// Fill out your copyright notice in the Description page of Project Settings.


#include "AITask/XBTTask_SeekingFocus.h"
#include "AI/XAIController.h"
#include "AI/XAI_Character.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UXBTTask_SeekingFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AXAIController* AICon = Cast<AXAIController>(OwnerComp.GetAIOwner());
	if (AICon)
	{
		UE_LOG(LogTemp, Warning, TEXT("Vector"));
		FVector Loc = AICon->GetBlackboardComponent()->GetValueAsVector(TargetPoint.SelectedKeyName);
		AICon->SetFocalPoint(Loc, EAIFocusPriority::LastFocusPriority);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
