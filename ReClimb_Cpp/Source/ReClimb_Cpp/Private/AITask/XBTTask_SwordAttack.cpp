// Fill out your copyright notice in the Description page of Project Settings.


#include "AITask/XBTTask_SwordAttack.h"
#include "AI/XAIController.h"
#include "AI/XAI_SwordCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UXBTTask_SwordAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AICon = Cast<AXAIController>(OwnerComp.GetAIOwner());
	//单播绑定，为了实现攻击完成或者移动失败后，返回行为树的结果
	FinishSuccessDelegate.BindUObject(this, &UXBTTask_SwordAttack::FinishSuccessEnd, &OwnerComp);
	FinishFailedDelegate.BindUObject(this, &UXBTTask_SwordAttack::FinishFailedEnd, &OwnerComp);

	if (AICon)
	{
		ControlledPawn = Cast<AXAI_SwordCharacter>(AICon->GetPawn());
		AttackActor = Cast<AActor>(AICon->GetBlackboardComponent()->GetValueAsObject(AttackTargetKey.SelectedKeyName));
		float Radius = AICon->GetBlackboardComponent()->GetValueAsFloat(AttackRadiusKey.SelectedKeyName);

		if (ControlledPawn && ControlledPawn->Implements<UXAIInterface>())
		{
			//通过AttackStart检测Token条件，如果不满足直接返回false，跳过攻击
			if (IXAIInterface::Execute_AttackStart(ControlledPawn, AttackActor, TokenNeeded))
			{
				//满足后，首先移动AI到攻击范围
				IXAIInterface::Execute_SetMovementSpeed(ControlledPawn, EAIMovement::EAM_Sprinting);
				AICon->ClearFocus(EAIFocusPriority::LastFocusPriority);
				AICon->ReceiveMoveCompleted.Clear();
				//绑定函数，处理AI是否成功到攻击范围
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
	//如果没有成功到达，那么直接调用AttackEnd函数返回占用的Token
	if (Result == EPathFollowingResult::Aborted)
	{
		if (ControlledPawn && ControlledPawn->Implements<UXAIInterface>())
		{
			UE_LOG(LogTemp, Warning, TEXT("MoveFaild"));
			IXAIInterface::Execute_AttackEnd(ControlledPawn, AttackActor);
			FinishFailedDelegate.ExecuteIfBound();
		}
	}
	//如果成功到达，那么执行攻击接口函数
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
		case ESwordAttackType::ESAT_SpinAttack:
			ControlledPawn->CallOnAttackEndCall.Clear();
			ControlledPawn->CallOnAttackEndCall.AddDynamic(this, &UXBTTask_SwordAttack::FinishAttack);
			ControlledPawn->SpinAttack(AttackActor);
			break;
		case ESwordAttackType::ESAT_GroundSmashAttack:
			ControlledPawn->CallOnAttackEndCall.Clear();
			ControlledPawn->CallOnAttackEndCall.AddDynamic(this, &UXBTTask_SwordAttack::FinishAttack);
			ControlledPawn->SmashAttack(AttackActor);
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
