// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Widget/XHUD.h"
#include "Struct/XStructInfo.h"
#include "Components/TimelineComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "XCombatComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackEnded);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RECLIMB_CPP_API UXCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UXCombatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY()
		AActor* CompOwner;
	UPROPERTY()
		class AXClimbCharacter* CharacterEx;
	//HUD
	UPROPERTY()
		class AXHUD* XCharacterHUD;
	UPROPERTY()
		class APlayerController* PIC;

	//准星
	UPROPERTY(EditAnywhere, Category = Crosshairs)
		class UTexture2D* CrosshairCenter;
	UPROPERTY(EditAnywhere, Category = Crosshairs)
		UTexture2D* CrosshairLeft;
	UPROPERTY(EditAnywhere, Category = Crosshairs)
		UTexture2D* CrosshairRight;
	UPROPERTY(EditAnywhere, Category = Crosshairs)
		UTexture2D* CrosshairTop;
	UPROPERTY(EditAnywhere, Category = Crosshairs)
		UTexture2D* CrosshairBottom;
	UPROPERTY(EditAnywhere, Category = Crosshairs)
		FHUDPackage HUDPackage;

	void SetHUDCrossHair(float delta);

	//spawn MagicSpell
public:

	FAttackInfo AttackInfo;
	UFUNCTION()
		void OnNotifyMontage(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
	UFUNCTION()
		void OnAttackMontageEnd(UAnimMontage* Montage, bool bInterrupted);

	//接受伤害事件
	FOnAttackEnded OnAttackEnded;

	void MagicSpell(FTransform& spawntransform, AActor* TargetActor, FDamageInfo damageinfo, TSubclassOf<class AXProjectilesBase> spawnprojclass);
	void FireBullet(FVector TraceStart, FVector TraceEnd, FDamageInfo damageinfo, AActor* IgnoreActor);
	void SwordAttack(FVector TraceStart, FVector TraceEnd, FDamageInfo damageinfo, AActor* IgnoreActor);
	TArray<AActor*> DamageAllNonTeamActor(TArray<FHitResult>& HitResults);
	AActor* DamageFirstNonTeamActor(TArray<FHitResult>& HitResults);
	//攻击信息
	FDamageInfo DamageInfo;
	//绑定击中函数
	UFUNCTION()
		void OnProjectileHit(AActor* OtherActor, const FHitResult& HitRes);

	//Smash Attack System
	void GroundSmash(FAttackInfo& attackinfo, float smashradius);
	UPROPERTY(EditAnywhere, Category = "SmashAttack")
		TSubclassOf<class AXAOEBase> SmashAOEClass;
	UPROPERTY()
		AXAOEBase* SmashAOEActor;
	float SmashRadius;
	void SmashAOE(float radius);

	UFUNCTION()
		void AOEDamageForOverlapActor(AActor* actor);

	//sword short
	void SwordPrimaryAttack(FAttackInfo& attackinfo, float length, float radius);
	void SwordPrimaryDamage();
	float SwordLength;
	float SwordRadius;

	//sword Jump
	void JumpPrimaryAttack(FAttackInfo& attackinfo);
	//预测玩家位置
	FVector PredicPlayerLoc(AActor* player, float pretime = 1.0f);
	void JumpPrimaryDamage();
	//跳跃攻击结束后，落地广播
	UFUNCTION()
		void OnLand(const FHitResult& Hit);

	//spin mesh
	void SpinMesh();
	FRotator LastRelRotation;
	//timeline spin
	UPROPERTY()
		UTimelineComponent* SpinTimeline;
	UPROPERTY(EditAnywhere)
		UCurveFloat* SpinFloatCurve;
	//曲线更新事件
	FOnTimelineFloat OnSpinTimelineTickCallBack;
	//完成事件
	FOnTimelineEvent OnSpinTimelineFinishedCallBack;
	UFUNCTION()
		void SpinTimelineTickCall(float value);
	UFUNCTION()
		void SpinTimelineFinishedCall();

	//Spin Chase
	UFUNCTION()
		void ChaseAttackTarget();
	//move to attack target
	UFUNCTION()
		void MoveEndCall(FAIRequestID RequestID, EPathFollowingResult::Type Result);
	//Delay Loop
	FTimerHandle ChaseLoopTimer;

	//spin Attack
	void SpinAttack(FAttackInfo& attackinfo, float radius);
	float SpinRadius;
	UPROPERTY(EditAnywhere, Category = "SmashAttack")
		TSubclassOf<AXAOEBase> SpinAOEClass;
	UPROPERTY()
		AXAOEBase* SpinAOEActor;
	void SpinAOE(float radius);


	//Mage Attack
	UPROPERTY()
		TSubclassOf<class AXProjectilesBase> AttackProjectileEx;
	UPROPERTY()
		TSubclassOf<AXProjectilesBase> NormalProjectileEx;
	void MageBaseAttack(FAttackInfo& attackinfo, TSubclassOf<AXProjectilesBase> attackprojectileEx);
	void MageBaseDamage();

	void MageBarrageAttack(FAttackInfo& attackinfo, TSubclassOf<AXProjectilesBase> normalprojectileEx);
	void MageBarrageDamage();
};
