// Fill out your copyright notice in the Description page of Project Settings.


#include "AITask/XBTTask_FocusTarget.h"
#include "AI/XAIController.h"
#include "AI/XAI_Character.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UXBTTask_FocusTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AXAIController* AICon = Cast<AXAIController>(OwnerComp.GetAIOwner());
	if (AICon)
	{
		AActor* Target = Cast<AActor>(AICon->GetBlackboardComponent()->GetValueAsObject(AttackTargetKey.SelectedKeyName));
		if (IsValid(Target))
		{
			UE_LOG(LogTemp, Warning, TEXT("Actor"));
			AICon->SetFocus(Target, EAIFocusPriority::LastFocusPriority);
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
