// Fill out your copyright notice in the Description page of Project Settings.


#include "AIDecorator/XBTDecorator_CanSeeTarget.h"
#include "AI/XAIController.h"
#include "AI/XAI_Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"

bool UXBTDecorator_CanSeeTarget::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AXAIController* AICon = Cast<AXAIController>(OwnerComp.GetAIOwner());
	if (AICon)
	{
		AXAI_Character* ControlledPawn = Cast<AXAI_Character>(AICon->GetPawn());
		AActor* Target = Cast<AActor>(AICon->GetBlackboardComponent()->GetValueAsObject(AttackTargetKey.SelectedKeyName));
		FVector Start = ControlledPawn->GetActorLocation();
		if (Target)
		{
			FVector End = Target->GetActorLocation();
			FHitResult HitRes;
			ETraceTypeQuery ETType = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility);
			bool bIsHit = UKismetSystemLibrary::LineTraceSingle(
				GetWorld(),
				Start,
				End,
				ETType,
				false,
				TArray<AActor*>{ControlledPawn},
				EDrawDebugTrace::ForDuration,
				HitRes,
				true
			);
			//如果检测到说明能看见
			if (bIsHit) return false;
		}
	}
	return true;
}
