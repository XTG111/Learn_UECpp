// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Struct/XStructInfo.h"
#include "DamageSystem/XDamageInterface.h"
#include "XClimbCharacter.generated.h"

UCLASS()
class RECLIMB_CPP_API AXClimbCharacter : public ACharacter, public IXDamageInterface
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

public:
	//��������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* MagicAttackMontage;
	//�ܻ�����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* OnHitMontage;

	//������ҵ��Ŷ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
		int TeamNum = 1;

private:
	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* TP_Camera;
	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* TP_CameraSpringArm;
	UPROPERTY(VisibleAnywhere)
		UArrowComponent* WallTraceArrow;

	//�������
	UPROPERTY(VisibleAnywhere)
		class UXClimbComponent* ClimbComponent;
	//���״̬���
	UPROPERTY(VisibleAnywhere)
		class UXPlayerStatsComponent* PlayerStatesComponent;
	//ս�����
	UPROPERTY(VisibleAnywhere)
		class UXCombatComponent* CombatComponent;
	//��ҿ�����
	UPROPERTY()
		class APlayerController* PIC;
	//HUD
	UPROPERTY()
		class AXHUD* XCharacterHUD;



	//��ҵ�ǰ��״̬
	EPlayerStance Stance;


	// �˶�����
	bool bCanMove = true;
	float AxisValue;
	//�Ƿ����
	bool bSprinting;
	//����״̬
	bool bInAttack;
	float SprintSpeed = 900.0f;
	float WalkSpeed = 250.0f;
	// ��ת�ǶȻ���
	float BaseTurnRate = 45.0f;
	//StopMontage����ʱTimerHandle;
	FTimerHandle StopMontageTimerHandle;
	FTimerDelegate StopMontageTimerDelegate;

	bool bAttacking;

public:
	inline EPlayerStance GetPlayerStance() { return Stance; }
	inline void SetPlayerStance(EPlayerStance stance) { Stance = stance; }
	inline bool GetInAttack() { return bInAttack; }
	// Damage Interface
public:
	float GetCurrentHealth_Implementation();
	float GetMaxHealth_Implementation();
	bool TakeDamage_Implementation(FDamageInfo DamageInfo, AActor* DamageCausor);
	float Heal_Implementation(float Amount);
	bool IsDead_Implementation();
	bool IsAttacking_Implementation();
	bool ReserveAttackToken_Implementation(int Amount);
	void ReturnAttackToken_Implementation(int Amount);
	int GetTeamNumber_Implementation();

public:
	/*
	*   ---    �ƶ�����    ---
	*/
	void MoveForWard(float value);
	void MoveRight(float value);
	void LookUp(float value);
	void Turn(float value);
	void DelayedStopMontageAndJumpPre();
	void JumpRe();
	void FastSpeed();
	void SlowSpeed();
	void Climb();
	void ReClimb();

	void Attack();
	void DoHeal();

	//State
public:

	//ֹͣ��̫�涯��
	void StopMontage(class UAnimMontage* Montage, float DelayBeforeStoppingMontage, float MontageBlendOutTime);
	UFUNCTION()
		void DelayStopMontage(float MontageBlendOutTime, UAnimMontage* Montage);

	/*
	*	---   ��ض�������   ---
	*/
	void OnLanded(const FHitResult& Hit);

	bool bInClimb;

	//�л�������Ĭ��״̬
	void MagicStance();
	void DefaultStance();

	//UFUNCTION Bind Delegate
public:
	UFUNCTION()
		void CallOnDeath();
	UFUNCTION()
		void CallOnBlocked(bool bCanbeParried);
	UFUNCTION()
		void CallOnDamageResponse(EDamageResponse DamageResponse, AActor* DamageCausor);

	//Montage Call
	UFUNCTION()
		void OnNotifyMontage(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
	UFUNCTION()
		void OnAttackMontageEnd(UAnimMontage* Montage, bool bInterrupted);
};
