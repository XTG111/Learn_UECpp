// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/XAI_Character.h"
#include "XAI_SwordCharacter.generated.h"

/**
 *
 */
UCLASS()
class RECLIMB_CPP_API AXAI_SwordCharacter : public AXAI_Character
{
	GENERATED_BODY()
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	void EquipWeapon_Implementation();
	void UnEquipWeapon_Implementation();
	void Attack_Implementation();


	/**     PlayMontage   **/
	//Attack

	UFUNCTION()
		void EndAttackMontage(UAnimMontage* Montage, bool bInterrupted);
	//Spawn Sword
	void WieldSword();
	UFUNCTION()
		void EndWieldMontage(UAnimMontage* Montage, bool bInterrupted);
	//Spawn Sword
	void SheathSword();
	UFUNCTION()
		void EndSheathMontage(UAnimMontage* Montage, bool bInterrupted);

};
