// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/Contexts/EnvQueryContext_BlueprintBase.h"
#include "EQS_Context_AttackTarget.generated.h"

/**
 *
 */
UCLASS()
class RECLIMB_CPP_API UEQS_Context_AttackTarget : public UEnvQueryContext_BlueprintBase
{
	GENERATED_BODY()
public:
	UEQS_Context_AttackTarget(const FObjectInitializer& ObjectInitializer);
	virtual UWorld* GetWorld() const override;
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;

};
