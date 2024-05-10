// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "XClimbCharacter.generated.h"

UCLASS()
class RECLIMB_CPP_API AXClimbCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AXClimbCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* TP_Camera;
	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* TP_CameraSpringArm;
	UPROPERTY(VisibleAnywhere)
		UArrowComponent* WallTraceArrow;
	UPROPERTY(VisibleAnywhere)
		UAnimInstance* AnimInstance;

	//攀爬组件
	UPROPERTY(VisibleAnywhere)
		class UXClimbComponent* ClimbComponent;

	// 运动输入
	float AxisValue;
	// 旋转角度基数
	float BaseTurnRate = 45.0f;
	//StopMontage的延时TimerHandle;
	FTimerHandle StopMontageTimerHandle;
	FTimerDelegate StopMontageTimerDelegate;
	//是否可以切换动作
	bool bCanChangeMontage = false;

public:
	/*
	*   ---    移动函数    --- 
	*/
	void MoveForWard(float value);
	void MoveRight(float value);
	void LookUp(float value);
	void Turn(float value);
	void JumpPre();
	void JumpRe();

	//停止蒙太奇动画
	void StopMontage(class UAnimMontage* Montage, float DelayBeforeStoppingMontage, float MontageBlendOutTime);
	UFUNCTION()
		void DelayStopMontage(float MontageBlendOutTime, UAnimMontage* Montage);
};
