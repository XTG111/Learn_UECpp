// Fill out your copyright notice in the Description page of Project Settings.


#include "AITask/XBTTask_MageAttack.h"
#include "AI/XAIController.h"
#include "AI/XAI_MageCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UXBTTask_MageAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AICon = Cast<AXAIController>(OwnerComp.GetAIOwner());
	//单播绑定，为了实现攻击完成或者移动失败后，返回行为树的结果
	FinishSuccessDelegate.BindUObject(this, &UXBTTask_MageAttack::FinishSuccessEnd, &OwnerComp);

	if (AICon)
	{
		ControlledPawn = Cast<AXAI_MageCharacter>(AICon->GetPawn());
		AttackActor = Cast<AActor>(AICon->GetBlackboardComponent()->GetValueAsObject(AttackTargetKey.SelectedKeyName));

		if (ControlledPawn && ControlledPawn->Implements<UXAIInterface>())
		{
			//通过AttackStart检测Token条件，如果不满足直接返回false，跳过攻击
			if (IXAIInterface::Execute_AttackStart(ControlledPawn, AttackActor, TokenNeeded))
			{
				AICon->SetFocus(AttackActor, EAIFocusPriority::LastFocusPriority);
				ControlledPawn = Cast<AXAI_MageCharacter>(AICon->GetPawn());
				if (bShouldTeleport)
				{
					FVector Loc = AICon->GetBlackboardComponent()->GetValueAsVector(TeleportLocation.SelectedKeyName);
					ControlledPawn->OnTeleportEnd.AddDynamic(this, &UXBTTask_MageAttack::TeleportEnd);
					ControlledPawn->Teleport(Loc);
				}
				else
				{
					TeleportEnd();
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

void UXBTTask_MageAttack::TeleportEnd()
{
	ControlledPawn->OnTeleportEnd.Clear();
	switch (AttackType)
	{
	case EMageAttackType::ESAT_Default:
		if (ControlledPawn && ControlledPawn->Implements<UXAIInterface>())
		{
			ControlledPawn->CallOnAttackEndCall.Clear();
			ControlledPawn->CallOnAttackEndCall.AddDynamic(this, &UXBTTask_MageAttack::FinishAttack);
			IXAIInterface::Execute_Attack(ControlledPawn, AttackActor);
		}
		break;
	case EMageAttackType::ESAT_BaseAttack:
		ControlledPawn->CallOnAttackEndCall.Clear();
		ControlledPawn->CallOnAttackEndCall.AddDynamic(this, &UXBTTask_MageAttack::FinishAttack);
		ControlledPawn->BaseAttack(AttackActor);
		break;
	case EMageAttackType::ESAT_BarrageAttack:
		ControlledPawn->CallOnAttackEndCall.Clear();
		ControlledPawn->CallOnAttackEndCall.AddDynamic(this, &UXBTTask_MageAttack::FinishAttack);
		ControlledPawn->BarrageAttack(AttackActor);
		break;
	case EMageAttackType::ESAT_GroundSmashAttack:
		ControlledPawn->CallOnAttackEndCall.Clear();
		ControlledPawn->CallOnAttackEndCall.AddDynamic(this, &UXBTTask_MageAttack::FinishAttack);
		ControlledPawn->SmashAttack(AttackActor);
		break;
	default:
		break;
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
