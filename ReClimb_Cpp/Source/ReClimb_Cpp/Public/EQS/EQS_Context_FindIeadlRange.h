// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/Contexts/EnvQueryContext_BlueprintBase.h"
#include "EQS_Context_FindIeadlRange.generated.h"

/**
 *
 */
UCLASS()
class RECLIMB_CPP_API UEQS_Context_FindIeadlRange : public UEnvQueryContext_BlueprintBase
{
	GENERATED_BODY()
public:
	virtual UWorld* GetWorld() const override;
	UEQS_Context_FindIeadlRange(const FObjectInitializer& ObjectInitializer);
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;

};
