// Fill out your copyright notice in the Description page of Project Settings.


#include "AIServices/XBTService_UpdateDisToTarget.h"
#include "AI/XAIController.h"
#include "AI/XAI_Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"

void UXBTService_UpdateDisToTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AXAIController* AICon = Cast<AXAIController>(OwnerComp.GetAIOwner());
	if (AICon)
	{
		AXAI_Character* ControlledPawn = Cast<AXAI_Character>(AICon->GetPawn());
		AActor* AttackActor = Cast<AActor>(AICon->GetBlackboardComponent()->GetValueAsObject(AttackTarget.SelectedKeyName));
		float distance = UKismetMathLibrary::Vector_Distance(ControlledPawn->GetActorLocation(), AttackActor->GetActorLocation());
		AICon->GetBlackboardComponent()->SetValueAsFloat(DistanceToTarget.SelectedKeyName, distance);
	}
}
