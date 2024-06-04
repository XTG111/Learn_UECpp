// Fill out your copyright notice in the Description page of Project Settings.


#include "EQS/EQS_Context_AttackTarget.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "AI/XAIController.h"
#include "AI/XAI_Character.h"
#include <EnvironmentQuery/Items/EnvQueryItemType_Actor.h>
#include <Kismet/GameplayStatics.h>
#include "GameFramework/PlayerStart.h"

UEQS_Context_AttackTarget::UEQS_Context_AttackTarget(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
}

UWorld* UEQS_Context_AttackTarget::GetWorld() const
{
	return GetOuter()->GetWorld();
}

void UEQS_Context_AttackTarget::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	//Super::ProvideContext(QueryInstance, ContextData);
	UObject* QObject = QueryInstance.Owner.Get();
	AXAI_Character* ControlledPawn = Cast<AXAI_Character>(QObject);
	if (ControlledPawn)
	{
		AXAIController* AIC = Cast<AXAIController>(ControlledPawn->GetController());
		if (AIC)
		{
			AActor* AttackTarget = AIC->GetAttackTargetActor();
			if (AttackTarget)
			{
				UE_LOG(LogTemp, Warning, TEXT("AttackTarget"));
				UEnvQueryItemType_Actor::SetContextHelper(ContextData, AttackTarget);
			}
		}
	}
}