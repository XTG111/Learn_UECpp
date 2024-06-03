// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/XCharacterInitInterface.h"
#include "D:/UnrealProject/Learn_UECpp/Learn_UECpp/ReClimb_Cpp/Source/ReClimb_Cpp/XHeadFile/XEnumType.h"
#include "Struct/XStructInfo.h"
#include "XClimbComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RECLIMB_CPP_API UXClimbComponent : public UActorComponent, public IXCharacterInitInterface
{
	GENERATED_BODY()
public:
	// Sets default values for this component's properties
	UXClimbComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(VisibleAnywhere)
		class AXClimbCharacter* CharacterRef;
	UPROPERTY(VisibleAnywhere)
		APlayerController* ControllerRef;
	UPROPERTY(VisibleAnywhere)
		class UArrowComponent* Arrow;
	UPROPERTY(VisibleAnywhere)
		class UCapsuleComponent* CapsuleComponent;
	UPROPERTY(VisibleAnywhere)
		class UCharacterMovementComponent* CharacterMovementIns;
	UPROPERTY(VisibleAnywhere)
		class USkeletalMeshComponent* FPMeshIns;

	//存储蒙太奇的结构体
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "AnimStruct")
		FAnims FAnimations;

	//播放蒙太奇动画时禁止输入的延迟时间
	FTimerHandle DisableInputTimerHandle;

	//是否正在攀爬状态
	bool bClimbing = false;
	//验证是否可以Climb，当当前没有在攀爬状态下
	bool bVerifyClimbing = true;
	//自动攀爬
	bool bToggleSprintClimb = true;

	//是否可以攀爬
	bool bCanClimb;
	//是否是翻越围墙
	bool bFence;
	//是否开启翻越
	bool bCanVault = true;
	//标记是否检测到墙壁
	bool bIsWallHit;
	//对于当前这堵墙应该是攀爬还是翻越
	bool bShouldClimb;
	//如果高度还高，进行跳跃攀爬的动作
	bool bJumpClimb;

	//跳跃状态
	EClimbingTypes ClimbingTypes = EClimbingTypes::ECT_LowClimb;

	//是否时跳跃落地状态
	bool bJumpImpactLandingActive;
	//落地动画的类型，高处翻越，低处翻越
	bool bLandingAnimType;

	//掉落速度检测的最小值
	float FallCheckMinium = -1000.0f;
	//掉落速度检测的最大值
	float FallCheckMax = -1500.0f;
	//奔跑速度阈值
	float SprintingSpeed = 500.0f;
	//最高可以翻越的高度
	float MaxLowClimbHeight = 60.0f;
	//最高可以攀爬的高度
	float MaxHighClimbHeight = 180.0f;

	//可站立墙厚
	float WallThickness = -75.0f;
	//薄边限制的厚度
	float LedgeCheckDistance = -15.0f;
	//墙壁的厚度
	float FenceCheckDistance = -3.0f;

	FTimerHandle JumpDownDelayTimerHandle;

	//墙壁正常高度
	//翻越围墙时的Location控制
	float JumpFenceVaultDistanceOffset = 0.0f;
	FVector JumpFenceVaultHeightOffset = { 0.0f,0.0f,185.0f };
	//没有翻越而是起跳攀爬的Location控制
	float RunJumpClimbDistanceOffset = 0.0f;
	FVector RunJumpClimbHeightOffset = { 0.0f,0.0f,205.0f };
	//原地起跳的Location控制
	float JumpClimbDistanceOffset = 0.0f;
	FVector JumpClimbHeightOffset = { 0.0f,0.0f,200.0f };

	//墙壁较高
	//对于高围墙的翻越的Location控制
	float HighFenceVaultDistanceOffset = 0.0f;
	FVector HighFenceVaultHeightOffset = { 0.0f,0.0f,85.0f };
	//对于高围墙的攀爬的Location控制
	float RunHighClimbDistanceOffset = 0.0f;
	FVector RunHighClimbHeightOffset = { 0.0f,0.0f,50.0f };
	//对于高围墙的原地起跳攀爬的Location控制
	float HighClimbDistanceOffset = 0.0f;
	FVector HighClimbHeightOffset = { 0.0f,0.0f,0.0f };

	//墙壁较低
	//对于低围墙的攀爬的Location控制
	float RunLowClimbDistanceOffset = 0.0f;
	FVector RunLowClimbHeightOffset = { 0.0f,0.0f,40.0f };
	//对于低围墙的原地攀爬的Location控制
	float	LowClimbDistanceOffset = 0.0f;
	FVector LowClimbHeightOffset = { 0.0f,0.0f,21.0f };
	//对于低围墙的翻越的Location控制
	float RunLowVaultDistanceOffset = 0.0f;
	FVector RunLowVaultHeightOffset = { 0.0f,0.0f,-5.0f };
	//对于低围墙的原地翻越的Location控制
	float LowVaultDistanceOffset = 0.0f;
	FVector LowVaultHeightOffset = { 0.0f,0.0f,-21.0f };

	//翻越或者攀爬时 禁止YawRotation
	bool bYawRotationDisabled;
	//检测墙体宽度是否能够满足攀爬后站立
	bool bThickLedge;

	//墙体高度、位置和插值速度
	FVector WallHeight;
	FVector WallLocation;
	float InterpolationSpeed;
	//墙的法线
	FVector WallNormal;
	FVector WallTopLocation;
	FVector FarWallHeight;

	//FootIK
	float IKLeftFootOffset;
	float IKRightFootOffset;
	FRotator IKLeftFootRotation;
	FRotator IKRightFootRotation;

	FVector LeftLocation;
	FVector RightLocation;

	FVector LeftHandLocation;
	FVector RightHandLocation;

	FVector CaftLeftLocation;
	FVector CaftRightLocation;

	float IKHipOffset;

	bool bDebug;

	//获取成员变量
public:
	inline bool GetbClimbing() { return bClimbing; }
	inline AXClimbCharacter* GetCharacter() { return CharacterRef; }
	inline FVector GetWallHeight() { return WallHeight; }
	inline FVector GetWallLocation() { return WallLocation; }
	inline float GetInterpolationSpeed() { return InterpolationSpeed; }
	inline float GetIKLeftFootOffset() { return IKLeftFootOffset; }
	inline float GetIKRightFootOffset() { return IKRightFootOffset; }
	inline float GetIKHipOffset() { return IKHipOffset; }
	inline FRotator GetIKLeftFootRotation() { return IKLeftFootRotation; }
	inline FRotator GetIKRightFootRotation() { return IKRightFootRotation; }
	inline FVector GetLeftLocation() { return LeftLocation; }
	inline FVector GetRightLocation() { return RightLocation; }
	inline FVector GetLeftHandLocation() { return LeftHandLocation; }
	inline FVector GetRightHandLocation() { return RightHandLocation; }
	inline FVector GetCaftLeftLocation() { return CaftLeftLocation; }
	inline FVector GetCaftRightLocation() { return CaftRightLocation; }

	//重写IXCharacterInitInterface接口类中的函数
public:
	virtual void InitCharacter(ACharacter* Player, UArrowComponent* WallTraceArrow, UCapsuleComponent* Capsule, UCharacterMovementComponent* CharacterMovement) override;
	virtual void StopMontage() override;
	virtual void SetFPMesh(USkeletalMeshComponent* FPMesh) override;

	//Debug
	void FloatDebug(const FString& DebugText, const float DebugFloat, const FString& DebugFinalText);

	//射线检测相关
public:
	//正上方检测
	void OverHeadCheck();
	//检测墙壁
	bool GetWallTrace(FHitResult& Hit);
	//检测墙壁高度
	bool GetWallHeightTrace(FHitResult& Hit);
	//检测该墙壁的另一面位置，判断当前的墙壁爬上去后能否站立
	bool GetFarWallHeight(FVector& TraceLocation);
	//检测当前是否在攀爬一个薄边
	bool LedgeCheck();
	//检测是否是一个Fence
	bool FenceCheck();


public:
	//判断是否正在攀爬
	bool ClimbingCheck();
	//掉落速度检测
	bool FallVelocityCheck(const float Velocity2Check);
	//延迟蒙太奇动画时间后开启输入
	void EnableInput();
	//检测当前Actor是否在运动
	bool MovingCheck();
	//当翻越时如果需要更改Rotation禁止YAW Rotation
	void CheckYawRotation();
	//当墙壁太薄，攀爬上去后会自动跳下，延迟一段时间再开启碰撞等
	void JumpDownDelay();
	//检测当前Actor是不是在奔跑
	bool SprintCheck();
	//判断表面是否可以行走
	bool WalkAbleSurface(const FHitResult& Hit);
	//当检测到一个墙壁时，通过检测结果设置墙壁的数据
	bool SetWallTrace(const FHitResult& Hit, bool noclimb);
	//设置墙壁高度
	void SetWallHeightTrace(const FHitResult& Hit, bool isHit);
	//判断该墙壁可以攀爬还是可以直接翻越
	void WallHeightCheck();
	//判断当前的墙壁厚度是不是足够攀爬 设置ThickLedge
	void WallThicknessCheck();

	/*
	*	---   翻越过程中角色Location的变化   ---
	*/
	//设置角色位置转换的平滑
	void SetPlayerLocationSmooth(float playerdistanceoffset, const FVector& heightoffset);
	//设置角色Location的变化
	void SetPlayerLocation(float playerdistanceoffset, const FVector& heightoffset, float Alpha);
	//设置角色的绕着墙壁法线的旋转 当在翻越时
	void FaceWall(float RotationTime);

	/*
	* --   落地动画 --
	*/
	//确定当前Actor从高处平台落下播放的动画
	void ImpactLand(const FHitResult& Hit);
	//确定Actor攀爬和跳跃后播放的动画
	void ImpactLanding(const FHitResult& Hit);
	//Actor 跳跃落地动画 根据是否是跳跃落地 bJILA 和 跳跃状态设置对应的动画
	void JumpImpactLanding(bool bJILA, bool bLAT);


	/*
	* --   三种攀爬类型对应设置  --
	*/
	void JumpClimbs();
	void JumpClimAction();
	void FinishJumpClimb(bool bshouldplaylandingAnimation, bool blandinganimtype);

	void HighClimbs();
	void HighClimAction();
	void FinishHighClimb(bool bshouldplaylandingAnimation, bool blandinganimtype);

	void LowClimbs();
	void LowClimAction();
	void FinishLowClimb(bool bshouldplaylandingAnimation, bool blandinganimtype);

	/*
	*	---  攀爬功能主函数
	*/
	void ClimbOrVault();
	//判断是否可以支持墙壁站立
	//有效值 为 0 Thick，1 NotThick
	int TraceCaculation();

	//结束攀爬
	void FinishClimb(bool bshouldplaylandingAnimation, bool bvaultLanding);

	/*
	*	--- Foot IK ---
	*/
	FTimerHandle FootIKTimer;
	void StartFootIKTimer();
	UFUNCTION(BlueprintCallable)
		void CalculateFootIK();

	//设置IK位置
	float IKFootTrace(const FName& SocketName, FVector& OutHitLocation, FVector& OutHitNormal);
	FRotator CalculateFootRotation(const FVector& hitnormal);

	FVector LeftFootIK();
	FVector RightFootIK();

	void HipOffset(FVector& RightFootLocation, FVector& LeftFootLocation);

	/*
	*	--- Auto Climb ---
	*/
	FTimerHandle AutoClimbTimer;
	void StartAutoClimbTimer();
	UFUNCTION(BlueprintCallable)
		void AutoClimb();

};
