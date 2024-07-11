// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/XAI_Character.h"
#include "XAI_SwordCharacter.generated.h"

/**
 *
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBlockEnded);
UCLASS()
class RECLIMB_CPP_API AXAI_SwordCharacter : public AXAI_Character
{
	GENERATED_BODY()
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	FOnBlockEnded OnBlockEnded;

	void EquipWeapon_Implementation() override;
	void UnEquipWeapon_Implementation() override;
	void Attack_Implementation(AActor* AttakTarget) override;
	void GetIdealRange_Implementation(float& AttackRadius, float& DefendRadius) override;


	//Block Stace
	EBlockingStace BlockStace = EBlockingStace::EBS_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		class UAnimMontage* Block1Montage;
	//Block Success Play
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		class UAnimMontage* Block2Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		class UAnimMontage* JumpAttackMontage;

	//Block Damage
	void StartBlock();
	//use for animation block
	UFUNCTION()
		void EndBlockMontage(UAnimMontage* Montage, bool bInterrupted);
	//use for direct block
	UFUNCTION()
		void EndBlock();


	void CallOnBlocked(bool bCanbeParried);

	void CallOnDamageResponse(EDamageResponse DamageResponse, AActor* DamageCausor);

	/**     PlayMontage   **/
	//Attack
	UFUNCTION()
		void EndAttackMontage(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
		void OnNotifyMontage(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

	//Spawn Sword
	void WieldSword();
	UFUNCTION()
		void EndWieldMontage(UAnimMontage* Montage, bool bInterrupted);
	//Spawn Sword
	void SheathSword();

	UFUNCTION()
		void EndSheathMontage(UAnimMontage* Montage, bool bInterrupted);

	//override take damage function
	bool TakeDamage_Implementation(FDamageInfo DamageInfo, AActor* DamageCausor);
	//try to block same attack
	void TryToBlock();


public:
	//Control Block CoolDownTime
	bool bCanBlock = true;
	FTimerHandle CoolDownBlock;
	UFUNCTION()
		void SetbCanBlock();

	//contorl block
	UPROPERTY(EditAnywhere, Category = "BlockProbability")
		float BlockProbability = 0.8f;
	FTimerHandle BlockEndTimer;

	//增加攻击种类
	void ShortAttack(AActor* AttakTarget);
	void CauseDamage();
	void JumpAttack(AActor* AttakTarget);
	//将AIEnemy 移动到玩家位置
	void JumpToAttackTarget(AActor* AttakTarget);
	//跳跃攻击结束后，落地广播
	UFUNCTION()
		void OnLand(const FHitResult& Hit);
	//预测玩家位置
	FVector PredicPlayerLoc(AActor* player, float pretime = 1.0f);
};
