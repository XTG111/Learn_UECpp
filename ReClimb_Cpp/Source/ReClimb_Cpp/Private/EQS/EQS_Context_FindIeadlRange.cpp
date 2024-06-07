// Fill out your copyright notice in the Description page of Project Settings.


#include "EQS/EQS_Context_FindIeadlRange.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "AI/XAIController.h"
#include "AI/XAI_Character.h"
#include <EnvironmentQuery/Items/EnvQueryItemType_Actor.h>
#include <Kismet/GameplayStatics.h>
#include "GameFramework/PlayerStart.h"

UWorld* UEQS_Context_FindIeadlRange::GetWorld() const
{
	return GetOuter()->GetWorld();
}

UEQS_Context_FindIeadlRange::UEQS_Context_FindIeadlRange(const FObjectInitializer& ObjectInitializer)
{
}

void UEQS_Context_FindIeadlRange::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
}
