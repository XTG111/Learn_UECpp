// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <ReClimb_Cpp/XHeadFile/XEnumType.h>
#include "AIInterface/XAIInterface.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "XAI_Character.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackEnd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWieldSword);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSheathSword);
UCLASS()
class RECLIMB_CPP_API AXAI_Character : public ACharacter, public IXAIInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AXAI_Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* SwordBegin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* SwordEnd;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* AttackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class AXLineBase* Patrol;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class AXSwordBase* Sword;

	bool bIsWiledSword = false;
public:
	inline bool GetIsWiledSword() { return bIsWiledSword; }

	//Interface
public:
	class AXLineBase* GetPatrolRoute_Implementation() override;
	float SetMovementSpeed_Implementation(EAIMovement SpeedEnum) override;

public:

	FOnAttackEnd CallOnAttackEndCall;
	FOnWieldSword CallOnWieldSword;
	FOnSheathSword CallOnSheathSword;

	/**     PlayMontage   **/
	//Attack
	void Attack();
	UFUNCTION()
		void EndAttackMontage(UAnimMontage* Montage, bool bInterrupted);
	//Spawn Sword
	void WireldSword();
	UFUNCTION()
		void EndWieldMontage(UAnimMontage* Montage, bool bInterrupted);
	//Spawn Sword
	void SheathSword();
	UFUNCTION()
		void EndSheathMontage(UAnimMontage* Montage, bool bInterrupted);
};
