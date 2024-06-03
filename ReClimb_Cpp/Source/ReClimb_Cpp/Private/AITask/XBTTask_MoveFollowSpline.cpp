// Fill out your copyright notice in the Description page of Project Settings.


#include "AITask/XBTTask_MoveFollowSpline.h"
#include "AI/XAIController.h"
#include "AI/XAI_Character.h"
#include "Other/XLineBase.h"

EBTNodeResult::Type UXBTTask_MoveFollowSpline::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AXAIController* AICon = Cast<AXAIController>(OwnerComp.GetAIOwner());
	if (AICon)
	{
		AXAI_Character* ControlledPawn = Cast<AXAI_Character>(AICon->GetPawn());
		if (ControlledPawn && ControlledPawn->Implements<UXAIInterface>())
		{
			AXLineBase* Patrol = IXAIInterface::Execute_GetPatrolRoute(ControlledPawn);
			if (Patrol)
			{
				//C
				FVector Location = Patrol->GetSplinePointAsWorldPosition();
				EPathFollowingRequestResult::Type type = AICon->MoveToLocation(Location, 10.0f, false);
				if (type == EPathFollowingRequestResult::AlreadyAtGoal)
				{
					Patrol->IncrementPatrolRoute();
					return EBTNodeResult::Succeeded;
				}
			}
		}
	}
	return EBTNodeResult::Failed;
}

EBTNodeResult::Type UXBTTask_MoveFollowSpline::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AXAIController* AICon = Cast<AXAIController>(OwnerComp.GetAIOwner());
	if (AICon)
	{
		AICon->StopMovement();
		FinishAbort();
	}
	return EBTNodeResult::Succeeded;
}
