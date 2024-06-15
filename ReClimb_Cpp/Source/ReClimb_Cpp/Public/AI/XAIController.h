// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "D:/UnrealProject/Learn_UECpp/Learn_UECpp/ReClimb_Cpp/Source/ReClimb_Cpp/XHeadFile/XEnumType.h"
#include "D:/runajian/epic_dw/ue4_27/UE_4.27/Engine/Source/Runtime/AIModule/Classes/Perception/AIPerceptionTypes.h"
#include "XAIController.generated.h"

/**
 *
 */
UCLASS()
class RECLIMB_CPP_API AXAIController : public AAIController
{
	GENERATED_BODY()
public:
	AXAIController();

private:
	UPROPERTY(EditAnywhere, Category = "AI")
		class UBehaviorTree* BehaviorTree;

	//UPROPERTY(EditAnywhere, Category = "AI")
	//	class UBlackboard* BlackboardData;

	// AIPerception 组件
	UPROPERTY(BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
		class UAIPerceptionComponent* AIPerceptionComponent;
	// AIPerception 配置（视觉）
	UPROPERTY(BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
		class UAISenseConfig_Sight* SightConfig;
	// AIPerception 配置（听觉）
	UPROPERTY(BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
		class UAISenseConfig_Hearing* HearingConfig;
	// AIPerception 配置（受击）
	UPROPERTY(BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
		class UAISenseConfig_Damage* DamageConfig;

	//AttackTarget
	UPROPERTY(BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
		AActor* AttackTargetActor;

	FName AttackTargetKeyName = TEXT("AttackTarget");
	FName AIStateKeyName = TEXT("AIState");
	FName PointOfInterestKeyName = TEXT("PointOfInterest");
	FName AttackRangeKeyName = TEXT("AttackRange");
	FName DefendRangeKeyName = TEXT("DefendRange");
public:
	inline UAIPerceptionComponent* GetAIPerceptionComponent() { return AIPerceptionComponent; }
	AActor* GetAttackTargetActor() { return AttackTargetActor; }

public:
	virtual void OnPossess(APawn* InPawn) override;

	virtual void BeginPlay() override;

	//设置当前AI的状态
	void SetStateAsPassive();
	void SetStateAsDead();
	void SetStateAsInvestigatinig(const FVector& Location);
	void SetStateAsAttacking(AActor* AttackTarget, bool bUseLastTarget);
	void SetStateAsFrozen();

	//AI Sense

	//Check Sense Result
	bool CanSenseActor(AActor* Actor, EAISense type, FAIStimulus& Simulus);
	//Get Current State
	EAIState GetCurrentState();
	void HandleSenseSound(const FVector& Location);
	void HandleSenseSight(AActor* Actor);
	void HandleSenseDamage(AActor* Actor);

	//绑定函数
	UFUNCTION()
		void PerceptionUpdated(const TArray<AActor*>& UpdatedActors);
};
