// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "XAI_MageAnimInstance.generated.h"

/**
 *
 */
UCLASS()
class RECLIMB_CPP_API UXAI_MageAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	//������ͼ��ʼ������
	virtual void NativeInitializeAnimation() override;

	//����Event Tick����
	virtual void NativeUpdateAnimation(float DeltaTime) override;
private:
	UPROPERTY(BlueprintReadOnly, Category = CharacterRef, meta = (AllowPrivateAccess = "true"))
		class AXAI_Character* XAINPC;
	//State Machine
	UPROPERTY(BlueprintReadOnly, Category = IsInAir, meta = (AllowPrivateAccess = "true"))
		bool bIsInAir;
	//1D Blend Space
	UPROPERTY(BlueprintReadOnly, Category = MoveControl, meta = (AllowPrivateAccess = "true"))
		float speed;
	//Direction
	UPROPERTY(BlueprintReadOnly, Category = MoveControl, meta = (AllowPrivateAccess = "true"))
		float Direction;
	//FocusingTarget
	UPROPERTY(BlueprintReadOnly, Category = FocusingTarget, meta = (AllowPrivateAccess = "true"))
		bool bIsFocusingTarget;
};
