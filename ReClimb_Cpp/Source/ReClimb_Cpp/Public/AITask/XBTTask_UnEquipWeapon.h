// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "XBTTask_UnEquipWeapon.generated.h"

/**
 *
 */
UCLASS()
class RECLIMB_CPP_API UXBTTask_UnEquipWeapon : public UBTTask_BlueprintBase
{
	GENERATED_BODY()
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	UFUNCTION()
		void Finish();
	UFUNCTION()
		void FinishLast(UBehaviorTreeComponent* OwnerComp);

	FSimpleDelegate FinishDelegate;

};
