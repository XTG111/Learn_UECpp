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

	//�������
	UPROPERTY(VisibleAnywhere)
		class UXClimbComponent* ClimbComponent;

	// �˶�����
	float AxisValue;
	// ��ת�ǶȻ���
	float BaseTurnRate = 45.0f;
	//StopMontage����ʱTimerHandle;
	FTimerHandle StopMontageTimerHandle;
	FTimerDelegate StopMontageTimerDelegate;
	//�Ƿ�����л�����
	bool bCanChangeMontage = false;

public:
	/*
	*   ---    �ƶ�����    --- 
	*/
	void MoveForWard(float value);
	void MoveRight(float value);
	void LookUp(float value);
	void Turn(float value);
	void JumpPre();
	void JumpRe();

	//ֹͣ��̫�涯��
	void StopMontage(class UAnimMontage* Montage, float DelayBeforeStoppingMontage, float MontageBlendOutTime);
	UFUNCTION()
		void DelayStopMontage(float MontageBlendOutTime, UAnimMontage* Montage);
};
