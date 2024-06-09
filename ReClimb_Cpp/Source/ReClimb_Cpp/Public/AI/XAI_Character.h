// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <ReClimb_Cpp/XHeadFile/XEnumType.h>
#include "AIInterface/XAIInterface.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "XAI_Character.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackEnd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEquipWeapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUnEquipWeapon);
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

	UFUNCTION()
		void ReceivedDamage(AActor* DamageActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* SwordBegin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* SwordEnd;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* AttackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class AXLineBase* Patrol;

	UPROPERTY(EditAnywhere, Category = "AI")
		class UBehaviorTree* BehaviorTree;


	bool bIsWiledWeapon = false;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor> Sword;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AXSwordBase* Weapon;

	inline bool GetIsWiledWeapon() { return bIsWiledWeapon; }

	UFUNCTION(BlueprintCallable)
		void SetIsWiledWeapon(bool bSet);

	//Interface
public:
	class AXLineBase* GetPatrolRoute_Implementation() override;
	float SetMovementSpeed_Implementation(EAIMovement SpeedEnum) override;
	void GetIdealRange_Implementation(float& AttackRadius, float& DefendRadius) override;
	void EquipWeapon_Implementation() override;
	void UnEquipWeapon_Implementation() override;
	void Attack_Implementation() override;
	float GetCurrentHealth_Implementation() override;
	float GetMaxHealth_Implementation() override;
	void Heal_Implementation(float healpercent) override;
public:

	FOnAttackEnd CallOnAttackEndCall;
	FOnEquipWeapon CallOnEquipWeapon;
	FOnUnEquipWeapon CallOnUnEquipWeapon;

private:
	UPROPERTY(VisibleAnywhere)
		class UXPlayerStatsComponent* AIStatesComponent;
};
