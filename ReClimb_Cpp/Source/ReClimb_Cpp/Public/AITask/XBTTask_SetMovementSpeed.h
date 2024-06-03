// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "D:/UnrealProject/Learn_UECpp/Learn_UECpp/ReClimb_Cpp/Source/ReClimb_Cpp/XHeadFile/XEnumType.h"
#include "XBTTask_SetMovementSpeed.generated.h"

/**
 *
 */
UCLASS()
class RECLIMB_CPP_API UXBTTask_SetMovementSpeed : public UBTTask_BlueprintBase
{
	GENERATED_BODY()
public:
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EAIMovement SpeedEnum;
};
