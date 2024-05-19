// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "XClimbCharacterAnimInstance.generated.h"

/**
 *
 */
UCLASS()
class RECLIMB_CPP_API UXClimbCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:

	//������ͼ��ʼ������
	virtual void NativeInitializeAnimation() override;

	//����Event Tick����
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	void FootIK();
private:
	UPROPERTY(BlueprintReadOnly, Category = CharacterRef, meta = (AllowPrivateAccess = "true"))
		class AXClimbCharacter* XClimbCharacter;
	//State Machine
	UPROPERTY(BlueprintReadOnly, Category = IsInAir, meta = (AllowPrivateAccess = "true"))
		bool bIsInAir;
	//1D Blend Space
	UPROPERTY(BlueprintReadOnly, Category = MoveControl, meta = (AllowPrivateAccess = "true"))
		float speed;
	//Direction
	UPROPERTY(BlueprintReadOnly, Category = MoveControl, meta = (AllowPrivateAccess = "true"))
		float Direction;
	//FootIK
	UPROPERTY(BlueprintReadOnly, Category = FootIK, meta = (AllowPrivateAccess = "true"))
		FVector LeftFootLocationIK;
	UPROPERTY(BlueprintReadOnly, Category = FootIK, meta = (AllowPrivateAccess = "true"))
		FVector RightFootLocationIK;
	UPROPERTY(BlueprintReadOnly, Category = FootIK, meta = (AllowPrivateAccess = "true"))
		FVector CaftLeftLocationIK;
	UPROPERTY(BlueprintReadOnly, Category = FootIK, meta = (AllowPrivateAccess = "true"))
		FVector CaftRightLocationIK;
	UPROPERTY(BlueprintReadOnly, Category = FootIK, meta = (AllowPrivateAccess = "true"))
		float HipOffsetIK;
	UPROPERTY(BlueprintReadOnly, Category = FootIK, meta = (AllowPrivateAccess = "true"))
		FRotator LeftFootRotationIK;
	UPROPERTY(BlueprintReadOnly, Category = FootIK, meta = (AllowPrivateAccess = "true"))
		FRotator RightFootRotationIK;

};
