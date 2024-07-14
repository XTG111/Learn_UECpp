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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		class UAnimMontage* MagicAttackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		class UAnimMontage* MeleeAttackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		class UAnimMontage* BlockAttackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		class UAnimMontage* BlockReactMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		class UAnimMontage* ParryReactMontage;

	UPROPERTY(EditAnywhere, Category = "ComboAttack")
		TSubclassOf<class AXAOEBase> ComboAOEClass;
	UPROPERTY()
		AXAOEBase* ComboAOEActor;

	//受击动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
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
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AXProjectilesBase> ProjectileEx;
	//玩家控制器
	UPROPERTY()
		class APlayerController* PIC;
	//HUD
	UPROPERTY()
		class AXHUD* XCharacterHUD;

	//Sword And Shield
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> SwordClass;
	UPROPERTY()
		AActor* SwordActor;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> ShieldClass;
	UPROPERTY()
		AActor* ShieldActor;

	//玩家当前的状态
	EPlayerStance Stance;

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
	//是否在Combo
	bool bInComboWindow;
	bool bCanResumeCombo;
	//是否在Parr窗口
	bool bInParryWindow;
	bool bIsReactingBlock;


public:
	bool bAttacking;
	bool bBlocking;
	bool bCanMove = true;
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
	void SetIsInteruptible_Implementation(bool binter);
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

	void RightMousePress();
	void RightMouseRelease();

	//input to stance change
	void ChangeToUnarmed();
	void ChangeToMagic();
	void ChangeToMelee();

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
	void MeleeStance();
	void DefaultStance();
	void ChangeStance(EPlayerStance stance);

	//近战模式生成剑和盾
	void SpawnSwordAndShield();
	void SaveSwordAndShield();

	//Attack
	void MagicAttack();
	void MeleeAttack();
	UFUNCTION()
		void AOEDamageForOverlapActor(AActor* actor);

	//Block
	void StartBlock();
	void EndBlock();
	void ParryAttack(AActor* ParryActor);

	//UFUNCTION Bind Delegate
public:
	UFUNCTION()
		void CallOnDeath();
	UFUNCTION()
		void CallOnBlocked(AActor* DamageCauser, bool bCanbeParried);
	UFUNCTION()
		void CallOnDamageResponse(EDamageResponse DamageResponse, AActor* DamageCausor);

	//Montage Call
	UFUNCTION()
		void OnNotifyMontage(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
	UFUNCTION()
		void OnNotifyMontageEnd(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
	UFUNCTION()
		void OnAttackMontageEnd(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
		void OnBlockMontageEnd(UAnimMontage* Montage, bool bInterrupted);
};
