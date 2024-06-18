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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* Block1Montage;
	//Block Success Play
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* Block2Montage;

	//Block Damage
	void StartBlock();
	UFUNCTION()
		void EndBlockMontage(UAnimMontage* Montage, bool bInterrupted);

	virtual void CallOnBlocked_Implementation(bool bCanbeParried);
	virtual void CallOnDamageResponse_Implementation(EDamageResponse DamageResponse, AActor* DamageCausor);

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


public:
	//Control Block CoolDownTime
	bool bCanBlock = true;
	FTimerHandle CoolDownBlock;
	UFUNCTION()
		void SetbCanBlock();

};
