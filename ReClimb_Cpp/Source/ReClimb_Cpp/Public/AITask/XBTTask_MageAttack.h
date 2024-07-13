// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "Navigation/PathFollowingComponent.h"
#include "D:\UnrealProject\Learn_UECpp\Learn_UECpp\ReClimb_Cpp\Source\ReClimb_Cpp\XHeadFile\XEnumType.h"
#include "XBTTask_MageAttack.generated.h"

/**
 *
 */
UCLASS()
class RECLIMB_CPP_API UXBTTask_MageAttack : public UBTTask_BlueprintBase
{
	GENERATED_BODY()
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FBlackboardKeySelector AttackTargetKey;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FBlackboardKeySelector TeleportLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int TokenNeeded;
	UPROPERTY(EditAnywhere)
		EMageAttackType AttackType;

	UPROPERTY()
		AActor* AttackActor;
	UPROPERTY()
		class AXAI_MageCharacter* ControlledPawn;
	UPROPERTY()
		class AXAIController* AICon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bShouldTeleport = true;

	FSimpleDelegate FinishSuccessDelegate;

	UFUNCTION()
		void TeleportEnd();
	UFUNCTION()
		void FinishAttack();
	UFUNCTION()
		void FinishSuccessEnd(UBehaviorTreeComponent* OwnerComp);
};
