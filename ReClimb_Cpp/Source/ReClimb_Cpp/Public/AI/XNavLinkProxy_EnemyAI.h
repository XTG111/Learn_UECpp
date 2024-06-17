// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Navigation/NavLinkProxy.h"
#include "XNavLinkProxy_EnemyAI.generated.h"

/**
 *
 */
UCLASS()
class RECLIMB_CPP_API AXNavLinkProxy_EnemyAI : public ANavLinkProxy
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	UFUNCTION()
		void SmartLinkReached(AActor* Agent, const FVector& Destination);

};
