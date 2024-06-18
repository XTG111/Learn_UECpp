// Fill out your copyright notice in the Description page of Project Settings.


#include "AITask/XBTTask_GunAttack.h"
#include "AI/XAIController.h"
#include "AI/XAI_Character.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UXBTTask_GunAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AICon = Cast<AXAIController>(OwnerComp.GetAIOwner());
	//单播绑定，为了实现攻击完成或者移动失败后，返回行为树的结果
	FinishSuccessDelegate.BindUObject(this, &UXBTTask_GunAttack::FinishSuccessEnd, &OwnerComp);

	if (AICon)
	{
		ControlledPawn = Cast<AXAI_Character>(AICon->GetPawn());
		AttackActor = Cast<AActor>(AICon->GetBlackboardComponent()->GetValueAsObject(AttackTargetKey.SelectedKeyName));

		if (ControlledPawn && ControlledPawn->Implements<UXAIInterface>())
		{
			//通过AttackStart检测Token条件，如果不满足直接返回false，跳过攻击
			if (IXAIInterface::Execute_AttackStart(ControlledPawn, AttackActor, TokenNeeded))
			{
				AICon->SetFocus(AttackActor, EAIFocusPriority::LastFocusPriority);
				if (ControlledPawn && ControlledPawn->Implements<UXAIInterface>())
				{

					ControlledPawn->CallOnAttackEndCall.Clear();
					ControlledPawn->CallOnAttackEndCall.AddDynamic(this, &UXBTTask_GunAttack::FinishAttack);
					IXAIInterface::Execute_Attack(ControlledPawn, AttackActor);
				}
			}
			else
			{
				return EBTNodeResult::Failed;
			}
		}
	}
	return EBTNodeResult::InProgress;
}

void UXBTTask_GunAttack::FinishAttack()
{
	FinishSuccessDelegate.ExecuteIfBound();
}

void UXBTTask_GunAttack::FinishSuccessEnd(UBehaviorTreeComponent* OwnerComp)
{
	FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
}
