// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/XAI_Character.h"
#include "Components/TimelineComponent.h"
#include "Navigation/PathFollowingComponent.h"
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
	//void Attack_Implementation(AActor* AttakTarget) override;
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		class UAnimMontage* SpinAttackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		class UAnimMontage* SmashMontage;

	UPROPERTY(EditAnywhere, Category = "SpinAttack")
		TSubclassOf<class AXAOEBase> SpinAOEClass;
	UPROPERTY()
		AXAOEBase* SpinAOEActor;

	//Block Damage
	void StartBlock();
	//use for animation block
	UFUNCTION()
		void EndBlockMontage(UAnimMontage* Montage, bool bInterrupted);
	//use for direct block
	UFUNCTION()
		void EndBlock();


	void CallOnBlocked(AActor* DamageCauser, bool bCanbeParried);

	void CallOnDamageResponse(EDamageResponse DamageResponse, AActor* DamageCausor);

	/**     PlayMontage   **/
	//Attack

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

	//jumpattack
	void JumpAttack(AActor* AttakTarget);

	//spinattack
	void SpinAttack(AActor* AttakTarget);

	//SmashAttack
	void SmashAttack(AActor* AttakTarget);
};
