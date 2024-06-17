// Fill out your copyright notice in the Description page of Project Settings.


#include "AIServices/XBTService_StopAttack.h"
#include "AI/XAIController.h"
#include "AI/XAI_Character.h"
#include "BehaviorTree/BlackboardComponent.h"

void UXBTService_StopAttack::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AXAIController* AICon = Cast<AXAIController>(OwnerComp.GetAIOwner());
	if (AICon)
	{
		AXAI_Character* ControlledPawn = Cast<AXAI_Character>(AICon->GetPawn());
		AActor* AttackActor = Cast<AActor>(AICon->GetBlackboardComponent()->GetValueAsObject(AttackTarget.SelectedKeyName));
		if (AttackActor && AttackActor->Implements<UXDamageInterface>())
		{
			bool bIsDead = IXDamageInterface::Execute_IsDead(AttackActor);
			if (bIsDead)
			{
				AICon->SetStateAsPassive();
			}
		}
	}
}

