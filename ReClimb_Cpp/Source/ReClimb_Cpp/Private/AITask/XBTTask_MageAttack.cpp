// Fill out your copyright notice in the Description page of Project Settings.


#include "AITask/XBTTask_MageAttack.h"
#include "AI/XAIController.h"
#include "AI/XAI_MageCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UXBTTask_MageAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AICon = Cast<AXAIController>(OwnerComp.GetAIOwner());
	//�����󶨣�Ϊ��ʵ�ֹ�����ɻ����ƶ�ʧ�ܺ󣬷�����Ϊ���Ľ��
	FinishSuccessDelegate.BindUObject(this, &UXBTTask_MageAttack::FinishSuccessEnd, &OwnerComp);

	if (AICon)
	{
		ControlledPawn = Cast<AXAI_MageCharacter>(AICon->GetPawn());
		AttackActor = Cast<AActor>(AICon->GetBlackboardComponent()->GetValueAsObject(AttackTargetKey.SelectedKeyName));

		if (ControlledPawn && ControlledPawn->Implements<UXAIInterface>())
		{
			//ͨ��AttackStart���Token���������������ֱ�ӷ���false����������
			if (IXAIInterface::Execute_AttackStart(ControlledPawn, AttackActor, TokenNeeded))
			{
				AICon->SetFocus(AttackActor, EAIFocusPriority::LastFocusPriority);
				ControlledPawn = Cast<AXAI_MageCharacter>(AICon->GetPawn());
				FVector Loc = AICon->GetBlackboardComponent()->GetValueAsVector(TeleportLocation.SelectedKeyName);
				ControlledPawn->OnTeleportEnd.AddDynamic(this, &UXBTTask_MageAttack::TeleportEnd);
				ControlledPawn->Teleport(Loc);
			}
			else
			{
				return EBTNodeResult::Failed;
			}
		}
	}
	return EBTNodeResult::InProgress;
}

void UXBTTask_MageAttack::TeleportEnd()
{
	ControlledPawn->OnTeleportEnd.Clear();
	if (ControlledPawn && ControlledPawn->Implements<UXAIInterface>())
	{
		ControlledPawn->CallOnAttackEndCall.Clear();
		ControlledPawn->CallOnAttackEndCall.AddDynamic(this, &UXBTTask_MageAttack::FinishAttack);
		IXAIInterface::Execute_Attack(ControlledPawn, AttackActor);
	}
}

void UXBTTask_MageAttack::FinishAttack()
{
	FinishSuccessDelegate.ExecuteIfBound();
}

void UXBTTask_MageAttack::FinishSuccessEnd(UBehaviorTreeComponent* OwnerComp)
{
	FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
}
