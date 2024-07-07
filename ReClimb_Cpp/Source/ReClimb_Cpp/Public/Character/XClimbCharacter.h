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
	//攻击动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* MagicAttackMontage;
	//受击动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* OnHitMontage;

	//控制玩家的团队
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
		int TeamNum = 1;

private:
	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* TP_Camera;
	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* TP_CameraSpringArm;
	UPROPERTY(VisibleAnywhere)
		UArrowComponent* WallTraceArrow;

	//攀爬组件
	UPROPERTY(VisibleAnywhere)
		class UXClimbComponent* ClimbComponent;
	//玩家状态组件
	UPROPERTY(VisibleAnywhere)
		class UXPlayerStatsComponent* PlayerStatesComponent;
	//战斗组件
	UPROPERTY(VisibleAnywhere)
		class UXCombatComponent* CombatComponent;
	//玩家控制器
	UPROPERTY()
		class APlayerController* PIC;
	//HUD
	UPROPERTY()
		class AXHUD* XCharacterHUD;



	//玩家当前的状态
	EPlayerStance Stance;


	// 运动输入
	bool bCanMove = true;
	float AxisValue;
	//是否加速
	bool bSprinting;
	//攻击状态
	bool bInAttack;
	float SprintSpeed = 900.0f;
	float WalkSpeed = 250.0f;
	// 旋转角度基数
	float BaseTurnRate = 45.0f;
	//StopMontage的延时TimerHandle;
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
	*   ---    移动函数    ---
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

	//停止蒙太奇动画
	void StopMontage(class UAnimMontage* Montage, float DelayBeforeStoppingMontage, float MontageBlendOutTime);
	UFUNCTION()
		void DelayStopMontage(float MontageBlendOutTime, UAnimMontage* Montage);

	/*
	*	---   落地动画播放   ---
	*/
	void OnLanded(const FHitResult& Hit);

	bool bInClimb;

	//切换攻击和默认状态
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
