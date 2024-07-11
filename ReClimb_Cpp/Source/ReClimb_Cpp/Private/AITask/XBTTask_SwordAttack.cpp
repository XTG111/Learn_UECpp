// Fill out your copyright notice in the Description page of Project Settings.


#include "AITask/XBTTask_SwordAttack.h"
#include "AI/XAIController.h"
#include "AI/XAI_SwordCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UXBTTask_SwordAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AICon = Cast<AXAIController>(OwnerComp.GetAIOwner());
	//�����󶨣�Ϊ��ʵ�ֹ�����ɻ����ƶ�ʧ�ܺ󣬷�����Ϊ���Ľ��
	FinishSuccessDelegate.BindUObject(this, &UXBTTask_SwordAttack::FinishSuccessEnd, &OwnerComp);
	FinishFailedDelegate.BindUObject(this, &UXBTTask_SwordAttack::FinishFailedEnd, &OwnerComp);

	if (AICon)
	{
		ControlledPawn = Cast<AXAI_SwordCharacter>(AICon->GetPawn());
		AttackActor = Cast<AActor>(AICon->GetBlackboardComponent()->GetValueAsObject(AttackTargetKey.SelectedKeyName));
		float Radius = AICon->GetBlackboardComponent()->GetValueAsFloat(AttackRadiusKey.SelectedKeyName);

		if (ControlledPawn && ControlledPawn->Implements<UXAIInterface>())
		{
			//ͨ��AttackStart���Token���������������ֱ�ӷ���false����������
			if (IXAIInterface::Execute_AttackStart(ControlledPawn, AttackActor, TokenNeeded))
			{
				//����������ƶ�AI��������Χ
				IXAIInterface::Execute_SetMovementSpeed(ControlledPawn, EAIMovement::EAM_Sprinting);
				AICon->ClearFocus(EAIFocusPriority::LastFocusPriority);
				AICon->ReceiveMoveCompleted.Clear();
				//�󶨺���������AI�Ƿ�ɹ���������Χ
				AICon->ReceiveMoveCompleted.AddDynamic(this, &UXBTTask_SwordAttack::MoveEndCall);
				AICon->MoveToActor(AttackActor, Radius, false);
			}
			else
			{
				return EBTNodeResult::Failed;
			}
		}
	}
	return EBTNodeResult::InProgress;
}

void UXBTTask_SwordAttack::MoveEndCall(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	//���û�гɹ������ôֱ�ӵ���AttackEnd��������ռ�õ�Token
	if (Result == EPathFollowingResult::Aborted)
	{
		if (ControlledPawn && ControlledPawn->Implements<UXAIInterface>())
		{
			UE_LOG(LogTemp, Warning, TEXT("MoveFaild"));
			IXAIInterface::Execute_AttackEnd(ControlledPawn, AttackActor);
			FinishFailedDelegate.ExecuteIfBound();
		}
	}
	//����ɹ������ôִ�й����ӿں���
	else
	{
		AICon->SetFocus(AttackActor, EAIFocusPriority::LastFocusPriority);
		switch (AttackType)
		{
		case ESwordAttackType::ESAT_Default:
			if (ControlledPawn && ControlledPawn->Implements<UXAIInterface>())
			{

				ControlledPawn->CallOnAttackEndCall.Clear();
				ControlledPawn->CallOnAttackEndCall.AddDynamic(this, &UXBTTask_SwordAttack::FinishAttack);
				IXAIInterface::Execute_Attack(ControlledPawn, AttackActor);
			}
			break;
		case ESwordAttackType::ESAT_ShortAttack:
			ControlledPawn->CallOnAttackEndCall.Clear();
			ControlledPawn->CallOnAttackEndCall.AddDynamic(this, &UXBTTask_SwordAttack::FinishAttack);
			ControlledPawn->ShortAttack(AttackActor);
			break;
		case ESwordAttackType::ESAT_JumpAttack:
			ControlledPawn->CallOnAttackEndCall.Clear();
			ControlledPawn->CallOnAttackEndCall.AddDynamic(this, &UXBTTask_SwordAttack::FinishAttack);
			ControlledPawn->JumpAttack(AttackActor);
			break;
		default:
			break;
		}

	}
}

void UXBTTask_SwordAttack::FinishAttack()
{
	FinishSuccessDelegate.ExecuteIfBound();
}

void UXBTTask_SwordAttack::FinishSuccessEnd(UBehaviorTreeComponent* OwnerComp)
{
	FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
}

void UXBTTask_SwordAttack::FinishFailedEnd(UBehaviorTreeComponent* OwnerComp)
{
	FinishLatentTask(*OwnerComp, EBTNodeResult::Failed);
}
