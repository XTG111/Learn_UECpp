// Fill out your copyright notice in the Description page of Project Settings.


#include "AITask/XBTTask_DefaultAttack.h"
#include "AI/XAIController.h"
#include "AI/XAI_Character.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UXBTTask_DefaultAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AXAIController* AICon = Cast<AXAIController>(OwnerComp.GetAIOwner());
	if (AICon)
	{
		AXAI_Character* ControlledPawn = Cast<AXAI_Character>(AICon->GetPawn());
		if (ControlledPawn)
		{
			FinishDelegate.BindUObject(this, &UXBTTask_DefaultAttack::FinishLast, &OwnerComp);
			ControlledPawn->CallOnAttackEndCall.Clear();
			ControlledPawn->CallOnAttackEndCall.AddDynamic(this, &UXBTTask_DefaultAttack::Finish);
			if (ControlledPawn && ControlledPawn->Implements<UXAIInterface>())
			{
				AActor* TargetActor = Cast<AActor>(AICon->GetBlackboardComponent()->GetValueAsObject(AttackTargetKey.SelectedKeyName));
				IXAIInterface::Execute_Attack(ControlledPawn, TargetActor);
			}
		}
	}
	return EBTNodeResult::InProgress;
}

void UXBTTask_DefaultAttack::Finish()
{
	//UE_LOG(LogTemp, Warning, TEXT("FinishAttack"));
	FinishDelegate.ExecuteIfBound();
}

void UXBTTask_DefaultAttack::FinishLast(UBehaviorTreeComponent* OwnerComp)
{
	//UE_LOG(LogTemp, Warning, TEXT("FinishNode"));
	FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
}
