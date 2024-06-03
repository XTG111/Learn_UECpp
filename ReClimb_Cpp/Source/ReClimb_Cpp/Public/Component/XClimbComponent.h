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

	//�洢��̫��Ľṹ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "AnimStruct")
		FAnims FAnimations;

	//������̫�涯��ʱ��ֹ������ӳ�ʱ��
	FTimerHandle DisableInputTimerHandle;

	//�Ƿ���������״̬
	bool bClimbing = false;
	//��֤�Ƿ����Climb������ǰû��������״̬��
	bool bVerifyClimbing = true;
	//�Զ�����
	bool bToggleSprintClimb = true;

	//�Ƿ��������
	bool bCanClimb;
	//�Ƿ��Ƿ�ԽΧǽ
	bool bFence;
	//�Ƿ�����Խ
	bool bCanVault = true;
	//����Ƿ��⵽ǽ��
	bool bIsWallHit;
	//���ڵ�ǰ���ǽӦ�����������Ƿ�Խ
	bool bShouldClimb;
	//����߶Ȼ��ߣ�������Ծ�����Ķ���
	bool bJumpClimb;

	//��Ծ״̬
	EClimbingTypes ClimbingTypes = EClimbingTypes::ECT_LowClimb;

	//�Ƿ�ʱ��Ծ���״̬
	bool bJumpImpactLandingActive;
	//��ض��������ͣ��ߴ���Խ���ʹ���Խ
	bool bLandingAnimType;

	//�����ٶȼ�����Сֵ
	float FallCheckMinium = -1000.0f;
	//�����ٶȼ������ֵ
	float FallCheckMax = -1500.0f;
	//�����ٶ���ֵ
	float SprintingSpeed = 500.0f;
	//��߿��Է�Խ�ĸ߶�
	float MaxLowClimbHeight = 60.0f;
	//��߿��������ĸ߶�
	float MaxHighClimbHeight = 180.0f;

	//��վ��ǽ��
	float WallThickness = -75.0f;
	//�������Ƶĺ��
	float LedgeCheckDistance = -15.0f;
	//ǽ�ڵĺ��
	float FenceCheckDistance = -3.0f;

	FTimerHandle JumpDownDelayTimerHandle;

	//ǽ�������߶�
	//��ԽΧǽʱ��Location����
	float JumpFenceVaultDistanceOffset = 0.0f;
	FVector JumpFenceVaultHeightOffset = { 0.0f,0.0f,185.0f };
	//û�з�Խ��������������Location����
	float RunJumpClimbDistanceOffset = 0.0f;
	FVector RunJumpClimbHeightOffset = { 0.0f,0.0f,205.0f };
	//ԭ��������Location����
	float JumpClimbDistanceOffset = 0.0f;
	FVector JumpClimbHeightOffset = { 0.0f,0.0f,200.0f };

	//ǽ�ڽϸ�
	//���ڸ�Χǽ�ķ�Խ��Location����
	float HighFenceVaultDistanceOffset = 0.0f;
	FVector HighFenceVaultHeightOffset = { 0.0f,0.0f,85.0f };
	//���ڸ�Χǽ��������Location����
	float RunHighClimbDistanceOffset = 0.0f;
	FVector RunHighClimbHeightOffset = { 0.0f,0.0f,50.0f };
	//���ڸ�Χǽ��ԭ������������Location����
	float HighClimbDistanceOffset = 0.0f;
	FVector HighClimbHeightOffset = { 0.0f,0.0f,0.0f };

	//ǽ�ڽϵ�
	//���ڵ�Χǽ��������Location����
	float RunLowClimbDistanceOffset = 0.0f;
	FVector RunLowClimbHeightOffset = { 0.0f,0.0f,40.0f };
	//���ڵ�Χǽ��ԭ��������Location����
	float	LowClimbDistanceOffset = 0.0f;
	FVector LowClimbHeightOffset = { 0.0f,0.0f,21.0f };
	//���ڵ�Χǽ�ķ�Խ��Location����
	float RunLowVaultDistanceOffset = 0.0f;
	FVector RunLowVaultHeightOffset = { 0.0f,0.0f,-5.0f };
	//���ڵ�Χǽ��ԭ�ط�Խ��Location����
	float LowVaultDistanceOffset = 0.0f;
	FVector LowVaultHeightOffset = { 0.0f,0.0f,-21.0f };

	//��Խ��������ʱ ��ֹYawRotation
	bool bYawRotationDisabled;
	//���ǽ�����Ƿ��ܹ�����������վ��
	bool bThickLedge;

	//ǽ��߶ȡ�λ�úͲ�ֵ�ٶ�
	FVector WallHeight;
	FVector WallLocation;
	float InterpolationSpeed;
	//ǽ�ķ���
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

	//��ȡ��Ա����
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

	//��дIXCharacterInitInterface�ӿ����еĺ���
public:
	virtual void InitCharacter(ACharacter* Player, UArrowComponent* WallTraceArrow, UCapsuleComponent* Capsule, UCharacterMovementComponent* CharacterMovement) override;
	virtual void StopMontage() override;
	virtual void SetFPMesh(USkeletalMeshComponent* FPMesh) override;

	//Debug
	void FloatDebug(const FString& DebugText, const float DebugFloat, const FString& DebugFinalText);

	//���߼�����
public:
	//���Ϸ����
	void OverHeadCheck();
	//���ǽ��
	bool GetWallTrace(FHitResult& Hit);
	//���ǽ�ڸ߶�
	bool GetWallHeightTrace(FHitResult& Hit);
	//����ǽ�ڵ���һ��λ�ã��жϵ�ǰ��ǽ������ȥ���ܷ�վ��
	bool GetFarWallHeight(FVector& TraceLocation);
	//��⵱ǰ�Ƿ�������һ������
	bool LedgeCheck();
	//����Ƿ���һ��Fence
	bool FenceCheck();


public:
	//�ж��Ƿ���������
	bool ClimbingCheck();
	//�����ٶȼ��
	bool FallVelocityCheck(const float Velocity2Check);
	//�ӳ���̫�涯��ʱ���������
	void EnableInput();
	//��⵱ǰActor�Ƿ����˶�
	bool MovingCheck();
	//����Խʱ�����Ҫ����Rotation��ֹYAW Rotation
	void CheckYawRotation();
	//��ǽ��̫����������ȥ����Զ����£��ӳ�һ��ʱ���ٿ�����ײ��
	void JumpDownDelay();
	//��⵱ǰActor�ǲ����ڱ���
	bool SprintCheck();
	//�жϱ����Ƿ��������
	bool WalkAbleSurface(const FHitResult& Hit);
	//����⵽һ��ǽ��ʱ��ͨ�����������ǽ�ڵ�����
	bool SetWallTrace(const FHitResult& Hit, bool noclimb);
	//����ǽ�ڸ߶�
	void SetWallHeightTrace(const FHitResult& Hit, bool isHit);
	//�жϸ�ǽ�ڿ����������ǿ���ֱ�ӷ�Խ
	void WallHeightCheck();
	//�жϵ�ǰ��ǽ�ں���ǲ����㹻���� ����ThickLedge
	void WallThicknessCheck();

	/*
	*	---   ��Խ�����н�ɫLocation�ı仯   ---
	*/
	//���ý�ɫλ��ת����ƽ��
	void SetPlayerLocationSmooth(float playerdistanceoffset, const FVector& heightoffset);
	//���ý�ɫLocation�ı仯
	void SetPlayerLocation(float playerdistanceoffset, const FVector& heightoffset, float Alpha);
	//���ý�ɫ������ǽ�ڷ��ߵ���ת ���ڷ�Խʱ
	void FaceWall(float RotationTime);

	/*
	* --   ��ض��� --
	*/
	//ȷ����ǰActor�Ӹߴ�ƽ̨���²��ŵĶ���
	void ImpactLand(const FHitResult& Hit);
	//ȷ��Actor��������Ծ�󲥷ŵĶ���
	void ImpactLanding(const FHitResult& Hit);
	//Actor ��Ծ��ض��� �����Ƿ�����Ծ��� bJILA �� ��Ծ״̬���ö�Ӧ�Ķ���
	void JumpImpactLanding(bool bJILA, bool bLAT);


	/*
	* --   �����������Ͷ�Ӧ����  --
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
	*	---  ��������������
	*/
	void ClimbOrVault();
	//�ж��Ƿ����֧��ǽ��վ��
	//��Чֵ Ϊ 0 Thick��1 NotThick
	int TraceCaculation();

	//��������
	void FinishClimb(bool bshouldplaylandingAnimation, bool bvaultLanding);

	/*
	*	--- Foot IK ---
	*/
	FTimerHandle FootIKTimer;
	void StartFootIKTimer();
	UFUNCTION(BlueprintCallable)
		void CalculateFootIK();

	//����IKλ��
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
