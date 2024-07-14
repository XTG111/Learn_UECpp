// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Struct/XStructInfo.h"
#include "AIInterface/XAIInterface.h"
#include "DamageSystem/XDamageInterface.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "XAI_Character.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackEnd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEquipWeapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUnEquipWeapon);
UCLASS()
class RECLIMB_CPP_API AXAI_Character : public ACharacter, public IXAIInterface, public IXDamageInterface
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

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* SwordBegin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* SwordEnd;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* AttackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* HitMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* StaggerMontage;
	UPROPERTY(VisibleAnywhere)
		class AXLineBase* Patrol;

	UPROPERTY(VisibleAnywhere, Category = "AI")
		class AXAIController* AIController;

	UPROPERTY(EditAnywhere, Category = "AI")
		class UBehaviorTree* BehaviorTree;


	bool bIsWiledWeapon = false;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor> Sword;
	UPROPERTY(VisibleAnywhere)
		class AXSwordBase* Weapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UUserWidget> HPWidget;
	UPROPERTY(VisibleAnywhere)
		class UWidgetComponent* EnemyHPWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
		int TeamNum = 2;

	UPROPERTY()
		AActor* MakeDamageActor;
	UPROPERTY()
		AActor* AttackTargetActor;
	//存储AttackTarget和对其消耗的Token
	TMap<AActor*, int> CostTokenForTarget;
	//当前这次攻击需要消耗的Token
	UPROPERTY()
		int CurAttackNeedToken;

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
	void Attack_Implementation(AActor* AttakTarget) override;
	void JumpToLoc_Implementation(FVector Location) override;


	//DamageInterface
	float GetCurrentHealth_Implementation();
	float GetMaxHealth_Implementation();
	bool TakeDamage_Implementation(FDamageInfo DamageInfo, AActor* DamageCausor);
	float Heal_Implementation(float Amount);
	bool IsDead_Implementation();
	bool IsAttacking_Implementation();
	bool ReserveAttackToken_Implementation(int Amount);
	void ReturnAttackToken_Implementation(int Amount);
	void SetIsInteruptible_Implementation(bool binter);
	//设置团队
	int GetTeamNumber_Implementation();
	bool AttackStart_Implementation(AActor* AttackTarget, int TokenNeeded);
	void AttackEnd_Implementation(AActor* AttackTarget);
	void StoreAttackToken_Implementation(AActor* AttackTarget, int TokenNeeded);

public:


	FOnAttackEnd CallOnAttackEndCall;
	FOnEquipWeapon CallOnEquipWeapon;
	FOnUnEquipWeapon CallOnUnEquipWeapon;

	//UFUNCTION Bind Delegate
public:
	//BlueprintNativeEvent
	UFUNCTION()
		void CallOnDeath();
	//virtual void CallOnDeath_Implementation();
	UFUNCTION()
		void CallOnBlocked(AActor* DamageCauser, bool bCanbeParried);
	//virtual void CallOnBlocked_Implementation(bool bCanbeParried);
	UFUNCTION()
		void CallOnDamageResponse(EDamageResponse DamageResponse, AActor* DamageCausor);
	//virtual void CallOnDamageResponse_Implementation(EDamageResponse DamageResponse, AActor* DamageCausor);
	UFUNCTION()
		void OnHitMontageEnd(UAnimMontage* Montage, bool bInterrupted);

public:
	UPROPERTY(VisibleAnywhere)
		class UXPlayerStatsComponent* AIStatesComponent;
	UPROPERTY(VisibleAnywhere)
		class UXCombatComponent* CombatComponent;

	bool bAttacking;

};
