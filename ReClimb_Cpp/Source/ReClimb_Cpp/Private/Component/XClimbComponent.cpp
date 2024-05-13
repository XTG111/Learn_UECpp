// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/XClimbComponent.h"
#include "Character/XClimbCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Climb/XClimbingManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"


// Sets default values for this component's properties
UXClimbComponent::UXClimbComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UXClimbComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	//������ʼ��
	CharacterRef = Cast<AXClimbCharacter>(GetOwner());
	if (CharacterRef)
	{
		ControllerRef = Cast<APlayerController>(CharacterRef->GetController());
	}
	
}


// Called every frame
void UXClimbComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UXClimbComponent::InitCharacter(ACharacter* Player, UArrowComponent* WallTraceArrow, UCapsuleComponent* Capsule, UCharacterMovementComponent* CharacterMovement)
{
	CharacterRef = Cast<AXClimbCharacter>(Player);
	Arrow = WallTraceArrow;
	CapsuleComponent = Capsule;
	CharacterMovementIns = CharacterMovement;

}

void UXClimbComponent::StopMontage()
{
}

void UXClimbComponent::SetFPMesh(USkeletalMeshComponent* FPMesh)
{
}

void UXClimbComponent::OverHeadCheck()
{
	if (!Arrow || !CharacterRef) return;
	//У�����Ϸ��Ƿ����赲
	FVector ArrowStart = Arrow->GetComponentLocation();
	FVector ArrowEnd = ArrowStart + FVector{0.0f, 0.0f, 150.0f};
	EObjectTypeQuery w = UEngineTypes::ConvertToObjectType(ECC_WorldStatic);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = { TEnumAsByte<EObjectTypeQuery>(w) };
	FHitResult ArrowHit;
	bool bIsArrowHit = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		ArrowStart,
		ArrowEnd,
		ObjectTypes,
		false,
		TArray<AActor*>{},
		EDrawDebugTrace::ForDuration,
		ArrowHit,
		true,
		FColor::Green,
		FColor::Blue,
		5.0f
	);
	if (bIsArrowHit)
	{
		bCanClimb = false;
	}
	//У�����Ϸ����ż�ͷǰ��ƫ��һ�ξ����Ƿ��赲
	FVector CharacterStart = CharacterRef->GetActorLocation() + FVector{0.0f, 0.0f, 250.0f};
	FVector CharacterEnd = CharacterStart + Arrow->GetForwardVector() * 150.0f;
	FHitResult CharacterHit;
	bool bIsCharacterHit = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		CharacterStart,
		CharacterEnd,
		ObjectTypes,
		false,
		TArray<AActor*>{},
		EDrawDebugTrace::ForDuration,
		ArrowHit,
		true,
		FColor::Green,
		FColor::Purple,
		5.0f
	);
	if (bIsCharacterHit)
	{
		bCanClimb = false;
	}
}

bool UXClimbComponent::GetWallTrace(FHitResult& Hit)
{
	if (SprintCheck())
	{
		FVector ArrowStart = Arrow->GetComponentLocation();
		FVector ArrowEnd = ArrowStart + Arrow->GetForwardVector() * 150.0f;
		EObjectTypeQuery w = UEngineTypes::ConvertToObjectType(ECC_WorldStatic);
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = { TEnumAsByte<EObjectTypeQuery>(w) };
		FHitResult ArrowHit;
		bool bIsArrowHit = UKismetSystemLibrary::LineTraceSingleForObjects(
			GetWorld(),
			ArrowStart,
			ArrowEnd,
			ObjectTypes,
			false,
			TArray<AActor*>{},
			EDrawDebugTrace::ForDuration,
			ArrowHit,
			true,
			FColor::Red,
			FColor::Green,
			5.0f
		);

		if (IsValid(ArrowHit.GetActor()))
		{
			bool HasTag = ArrowHit.GetActor()->ActorHasTag("NoClimb") || ArrowHit.GetComponent()->ComponentHasTag("NoClimb");
			if (HasTag)
			{
				return false;
			}
			else
			{
				Hit = ArrowHit;
				return bIsArrowHit;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		FVector ArrowStart = Arrow->GetComponentLocation();
		FVector ArrowEnd = ArrowStart + Arrow->GetForwardVector() * 80.0f;
		EObjectTypeQuery w = UEngineTypes::ConvertToObjectType(ECC_WorldStatic);
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = { TEnumAsByte<EObjectTypeQuery>(w) };
		FHitResult ArrowHit;
		bool bIsArrowHit = UKismetSystemLibrary::LineTraceSingleForObjects(
			GetWorld(),
			ArrowStart,
			ArrowEnd,
			ObjectTypes,
			false,
			TArray<AActor*>{},
			EDrawDebugTrace::ForDuration,
			ArrowHit,
			true,
			FColor::Red,
			FColor::Green,
			5.0f
		);
		Hit = ArrowHit;
		return bIsArrowHit;
	}
}

bool UXClimbComponent::GetWallHeightTrace(FHitResult& Hit)
{
	//ͨ��ϵ��-3 �ı���Ҫ���õ�ǽ����
	FVector Thick = UKismetMathLibrary::GetForwardVector(UKismetMathLibrary::MakeRotFromX(WallNormal)) * -3.0f;
	
	FVector WallStart = WallLocation + Thick + FVector{0.0f, 0.0f, 350.0f};
	FVector WallEnd = WallStart - FVector{0.0f, 0.0f, 350.0f};
	EObjectTypeQuery w = UEngineTypes::ConvertToObjectType(ECC_WorldStatic);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = { TEnumAsByte<EObjectTypeQuery>(w) };
	FHitResult WallHit;
	bool bIsHit = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		WallStart,
		WallEnd,
		ObjectTypes,
		false,
		TArray<AActor*>{},
		EDrawDebugTrace::ForDuration,
		WallHit,
		true,
		FColor::Red,
		FColor::Green,
		5.0f
	);
	WallTopLocation = WallHit.ImpactPoint;
	Hit = WallHit;
	return bIsHit;
}

bool UXClimbComponent::GetFarWallHeight(FVector& TraceLocation)
{
	FVector Thick = UKismetMathLibrary::GetForwardVector(UKismetMathLibrary::MakeRotFromX(WallNormal)) * WallThickness;

	FVector WallStart = WallLocation + Thick + FVector{0.0f, 0.0f, 250.0f};
	FVector WallEnd = WallStart - FVector{0.0f, 0.0f, 250.0f};
	EObjectTypeQuery w = UEngineTypes::ConvertToObjectType(ECC_WorldStatic);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = { TEnumAsByte<EObjectTypeQuery>(w) };
	FHitResult WallHit;
	bool bIsHit = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		WallStart,
		WallEnd,
		ObjectTypes,
		false,
		TArray<AActor*>{},
		EDrawDebugTrace::ForDuration,
		WallHit,
		true,
		FColor::Red,
		FColor::Purple,
		5.0f
	);
	TraceLocation = WallHit.Location;
	return bIsHit;
}

bool UXClimbComponent::LedgeCheck()
{
	FVector Thick = UKismetMathLibrary::GetForwardVector(UKismetMathLibrary::MakeRotFromX(WallNormal)) * LedgeCheckDistance;

	FVector WallStart = WallLocation + Thick + FVector{0.0f, 0.0f, 250.0f};
	FVector WallEnd = WallStart - FVector{0.0f, 0.0f, 250.0f};
	EObjectTypeQuery w = UEngineTypes::ConvertToObjectType(ECC_WorldStatic);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = { TEnumAsByte<EObjectTypeQuery>(w) };
	FHitResult WallHit;
	bool bIsHit = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		WallStart,
		WallEnd,
		ObjectTypes,
		false,
		TArray<AActor*>{},
		EDrawDebugTrace::ForDuration,
		WallHit,
		true,
		FColor::Red,
		FColor::Purple,
		5.0f
	);
	return bIsHit;
}

bool UXClimbComponent::FenceCheck()
{
	FVector Thick = UKismetMathLibrary::GetForwardVector(UKismetMathLibrary::MakeRotFromX(WallNormal)) * FenceCheckDistance;

	FVector WallStart = WallLocation + Thick + FVector{0.0f, 0.0f, 250.0f};
	FVector WallEnd = WallStart - FVector{0.0f, 0.0f, 250.0f};
	EObjectTypeQuery w = UEngineTypes::ConvertToObjectType(ECC_WorldStatic);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = { TEnumAsByte<EObjectTypeQuery>(w) };
	FHitResult WallHit;
	bool bIsHit = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		WallStart,
		WallEnd,
		ObjectTypes,
		false,
		TArray<AActor*>{},
		EDrawDebugTrace::ForDuration,
		WallHit,
		true,
		FColor::Red,
		FColor::Purple,
		5.0f
	);
	bFence = bIsHit;
	return bIsHit;
}

bool UXClimbComponent::ClimbingCheck()
{
	FHitResult Hit;
	bClimbing = GetWallTrace(Hit);
	return bClimbing;
}

bool UXClimbComponent::FallVelocityCheck(const float Velocity2Check)
{
	return CharacterRef->GetVelocity().Z < Velocity2Check;
}

void UXClimbComponent::EnableInput()
{
	CharacterRef->EnableInput(ControllerRef);
	CharacterRef->GetWorldTimerManager().ClearTimer(DisableInputTimerHandle);
}

bool UXClimbComponent::MovingCheck()
{
	return CharacterRef->GetCharacterMovement()->GetLastUpdateVelocity().Size() > 0.0f;
}

void UXClimbComponent::CheckYawRotation()
{
	if (!CharacterRef || !ControllerRef || !CapsuleComponent || !CharacterMovementIns) return;
	//��⵱ǰ�ƶ��Ƿ����� UseControllerRotationYaw
	if (CharacterRef->bUseControllerRotationYaw)
	{
		bYawRotationDisabled = true;
		CharacterRef->bUseControllerRotationYaw = false;
	}
	else
	{
		//��������������bYawRotationDisabled�Ƿ�Ϊtrue
		if (bYawRotationDisabled)
		{
			CharacterRef->bUseControllerRotationYaw = true;
		}
	}
}

void UXClimbComponent::JumpDownDelay()
{
	CharacterRef->GetWorldTimerManager().ClearTimer(JumpDownDelayTimerHandle);
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CharacterMovementIns->SetMovementMode(EMovementMode::MOVE_Walking, 0);
	bClimbing = false;
	CheckYawRotation();
	CharacterRef->EnableInput(ControllerRef);
	
}

bool UXClimbComponent::SprintCheck()
{
	return CharacterRef->GetCharacterMovement()->GetLastUpdateVelocity().Size() > SprintingSpeed;
}

bool UXClimbComponent::WalkAbleSurface(const FHitResult& Hit)
{
	return CharacterMovementIns->IsWalkable(Hit);
}

bool UXClimbComponent::SetWallTrace(const FHitResult& Hit, bool noclimb)
{
	if (noclimb)
	{
		WallLocation = Hit.Location;
		WallNormal = Hit.Normal;
		bIsWallHit = true;
		return bIsWallHit;
	}
	else
	{
		bIsWallHit = false;
		return bIsWallHit;
	}
}

void UXClimbComponent::SetWallHeightTrace(const FHitResult& Hit, bool isHit)
{
	if (isHit)
	{
		WallHeight = Hit.Location;
	}
}

void UXClimbComponent::WallHeightCheck()
{
	float h = (WallHeight - WallLocation).Z;
	if (h < MaxLowClimbHeight)
	{
		bShouldClimb = false;
	}
	else
	{
		if (h < MaxHighClimbHeight)
		{
			bShouldClimb = true;
		}
		else
		{
			//����߶�С��350����ô������������
			if (h < 350.0f)
			{
				bJumpClimb = true;
			}
			else
			{
				//��������
				bVerifyClimbing = false;
				bClimbing = false;
			}
		}
	}
}

void UXClimbComponent::WallThicknessCheck()
{
	float h = (WallHeight - FarWallHeight).Z;
	if (h > 60.0f)
	{
		bThickLedge = false;
	}
	else
	{
		bThickLedge = true;
	}
}

void UXClimbComponent::SetPlayerLocationSmooth(float playerdistanceoffset, const FVector& heightoffset)
{
	AXClimbingManager* XClimbingMananger = Cast<AXClimbingManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AXClimbCharacter::StaticClass()));
	if (XClimbingMananger)
	{
		XClimbingMananger->SetPlayerLocationTimeline(playerdistanceoffset, heightoffset, CharacterRef);
	}
}

void UXClimbComponent::SetPlayerLocation(float playerdistanceoffset, const FVector& heightoffset, float Alpha)
{
	if (!CharacterRef) return;
	FVector newLocation;
	FVector WallForwardLocation = WallLocation + UKismetMathLibrary::GetForwardVector(UKismetMathLibrary::MakeRotFromX(WallNormal))* playerdistanceoffset;
	WallForwardLocation.Z = (WallHeight - heightoffset).Z;
	newLocation = FMath::Lerp(CharacterRef->GetActorLocation(), WallForwardLocation, Alpha);
	CharacterRef->SetActorLocation(newLocation, true, false);
	return;
}

void UXClimbComponent::FaceWall(float RotationTime)
{
	if (!CharacterRef) return;
	FRotator newRotation;
	FRotator EndRoation = UKismetMathLibrary::MakeRotFromX(WallNormal);
	EndRoation.Roll = CharacterRef->GetActorRotation().Roll;
	EndRoation.Pitch = CharacterRef->GetActorRotation().Pitch;
	EndRoation.Yaw += 180.0f;

	newRotation = UKismetMathLibrary::RLerp(CharacterRef->GetActorRotation(), EndRoation, RotationTime, true);
	CharacterRef->SetActorRotation(EndRoation);
}

void UXClimbComponent::ImpactLand(const FHitResult& Hit)
{
	//���������ٶȲ��Ŷ�����̫��
	bool FVC = FallVelocityCheck(FallCheckMax);
	TArray<UAnimMontage*> LowLandMontage = FAnimations.LandAnims.LowLand;
	TArray<UAnimMontage*> HighLandMontage = FAnimations.LandAnims.HighLand;
	int LowLandIndex = UKismetMathLibrary::RandomInteger(LowLandMontage.Num());
	int HighLandIndex = UKismetMathLibrary::RandomInteger(HighLandMontage.Num());
	UAnimMontage* LowLandAnimMontage = LowLandMontage[LowLandIndex];
	UAnimMontage* HighLandAnimMontage = HighLandMontage[HighLandIndex];
	UAnimMontage* AnimMontage = FVC ? HighLandAnimMontage : LowLandAnimMontage;
	//������ض���
	CharacterRef->PlayAnimMontage(AnimMontage, 1.0f, NAME_None);

	//������̫��ʱ����ֹͣ����
	CharacterRef->DisableInput(ControllerRef);
	float DelayTime = CharacterRef->GetVelocity().Z >= FallCheckMax ? 1.067f : 1.8f;
	CharacterRef->GetWorldTimerManager().SetTimer(
		DisableInputTimerHandle, 
		this, 
		&UXClimbComponent::EnableInput, 
		DelayTime, 
		false
	);
}

void UXClimbComponent::ImpactLanding(const FHitResult& Hit)
{
	if (FallVelocityCheck(FallCheckMinium))
	{
		ImpactLand(Hit);
	}
}

void UXClimbComponent::JumpImpactLanding(bool bJILA, bool bLAT)
{
	if (!bJILA) return;

	TArray<UAnimMontage*> LowVaultLandMontage = FAnimations.LandAnims.LowVaultLand;
	TArray<UAnimMontage*> HighVaultLandMontage = FAnimations.LandAnims.HighVaultLand;
	int LowVaultLandIndex = UKismetMathLibrary::RandomInteger(LowVaultLandMontage.Num());
	int HighVaultLandIndex = UKismetMathLibrary::RandomInteger(HighVaultLandMontage.Num());
	UAnimMontage* LowVaultLandAnimMontage = LowVaultLandMontage[LowVaultLandIndex];
	UAnimMontage* HighVaultLandAnimMontage = HighVaultLandMontage[HighVaultLandIndex];
	UAnimMontage* AnimMontage = bLAT ? LowVaultLandAnimMontage : HighVaultLandAnimMontage;
	//������ض���
	CharacterRef->PlayAnimMontage(AnimMontage, 1.0f, NAME_None);
}

void UXClimbComponent::JumpClimbs()
{
	UE_LOG(LogTemp, Warning, TEXT("JumpClimbs!"));
	ClimbingTypes = EClimbingTypes::ECT_JumpClimb;
	JumpClimAction();
}

void UXClimbComponent::JumpClimAction()
{
	if (!CharacterRef || !ControllerRef || !CapsuleComponent) return;
	UE_LOG(LogTemp, Warning, TEXT("JumpClimbAction!"));
	bCanClimb = true;
	//�����Ϸ��Ƿ���������ж�
	OverHeadCheck();
	if (bCanClimb)
	{
		//��ֹ����
		CharacterRef->DisableInput(ControllerRef);
		//�رս�������ײ
		//CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CharacterMovementIns->SetMovementMode(EMovementMode::MOVE_Flying, 0);
		if (bFence)
		{
			//���ó�ʼ��Խλ��
			SetPlayerLocationSmooth(JumpFenceVaultDistanceOffset, JumpFenceVaultHeightOffset);
			//���ŷ�Խ�Ķ���
			int len = FAnimations.VaultAnims.JumpFenceVault.Num();
			UAnimMontage* AnimMontage = FAnimations.VaultAnims.JumpFenceVault[UKismetMathLibrary::RandomInteger(len)];
			CharacterRef->PlayAnimMontage(AnimMontage, 1.0f, NAME_None);
		}
		else
		{
			if (MovingCheck())
			{
				SetPlayerLocationSmooth(RunJumpClimbDistanceOffset, RunJumpClimbHeightOffset);
				int len = FAnimations.ClimbAnims.RunJumpClimb.Num();
				UAnimMontage* AnimMontage = FAnimations.ClimbAnims.RunJumpClimb[UKismetMathLibrary::RandomInteger(len)];
				CharacterRef->PlayAnimMontage(AnimMontage, 1.0f, NAME_None);
			}
			else
			{
				SetPlayerLocationSmooth(JumpClimbDistanceOffset, JumpClimbHeightOffset);
				int len = FAnimations.ClimbAnims.JumpClimb.Num();
				UAnimMontage* AnimMontage = FAnimations.ClimbAnims.JumpClimb[UKismetMathLibrary::RandomInteger(len)];
				CharacterRef->GetMesh()->GetAnimInstance()->Montage_Play(AnimMontage);
				bVerifyClimbing = true;
			}
		}
	}
	else
	{
		bClimbing = false;
	}
}

void UXClimbComponent::FinishJumpClimb(bool bshouldplaylandingAnimation, bool blandinganimtype)
{
	
	if (!CharacterRef || !ControllerRef || !CapsuleComponent || !CharacterMovementIns) return;
	UE_LOG(LogTemp, Warning, TEXT("FinishJumpClimb!"));
	if (bFence)
	{
		bFence = false;
		JumpImpactLanding(bshouldplaylandingAnimation, false);
		UE_LOG(LogTemp, Warning, TEXT("Fence Jump Finish!"));
		CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		CharacterMovementIns->SetMovementMode(EMovementMode::MOVE_Walking, 0);
		bClimbing = false;
		CheckYawRotation();
		CharacterRef->EnableInput(ControllerRef);
	}
	else
	{
		if (bshouldplaylandingAnimation)
		{
			JumpImpactLanding(bshouldplaylandingAnimation, blandinganimtype);
		}
		if (bThickLedge)
		{
			CharacterRef->EnableInput(ControllerRef);
			CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			CharacterMovementIns->SetMovementMode(EMovementMode::MOVE_Walking, 0);
			bClimbing = false;
			CheckYawRotation();
			CharacterRef->EnableInput(ControllerRef);
		}
		else
		{
			//�����ǰǽ���޷�վ��������������Ҫ�������µĶ���
			int len = FAnimations.VaultAnims.HighVault_JumpDown.Num();
			UAnimMontage* AnimMontage = FAnimations.VaultAnims.HighVault_JumpDown[UKismetMathLibrary::RandomInteger(len)];
			CharacterRef->PlayAnimMontage(AnimMontage, 1.0f, NAME_None);
			CharacterRef->GetWorldTimerManager().SetTimer(JumpDownDelayTimerHandle, this, &UXClimbComponent::JumpDownDelay, 2.7f, false);
		}
	}
}

void UXClimbComponent::HighClimbs()
{
	ClimbingTypes = EClimbingTypes::ECT_HighClimb;
	HighClimAction();
}

void UXClimbComponent::HighClimAction()
{
	if (!CharacterRef || !ControllerRef || !CapsuleComponent) return;
	bCanClimb = true;
	OverHeadCheck();
	if (bCanClimb)
	{
		CharacterRef->DisableInput(ControllerRef);
		//�رս�������ײ
		CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CharacterMovementIns->SetMovementMode(EMovementMode::MOVE_Flying, 0);
		if (bFence)
		{
			//���ó�ʼ��Խλ��
			SetPlayerLocationSmooth(HighFenceVaultDistanceOffset, HighFenceVaultHeightOffset);
			//���ŷ�Խ�Ķ���
			int len = FAnimations.VaultAnims.HighFenceVault.Num();
			UAnimMontage* AnimMontage = FAnimations.VaultAnims.HighFenceVault[UKismetMathLibrary::RandomInteger(len)];
			CharacterRef->PlayAnimMontage(AnimMontage, 1.0f, NAME_None);
		}
		else
		{
			if (MovingCheck())
			{
				SetPlayerLocationSmooth(RunHighClimbDistanceOffset, RunHighClimbHeightOffset);
				//���ŷ�Խ�Ķ���
				int len = FAnimations.ClimbAnims.RunHighClimb.Num();
				UAnimMontage* AnimMontage = FAnimations.ClimbAnims.RunHighClimb[UKismetMathLibrary::RandomInteger(len)];
				CharacterRef->PlayAnimMontage(AnimMontage, 1.0f, NAME_None);
			}
			else
			{
				SetPlayerLocationSmooth(HighClimbDistanceOffset,HighClimbHeightOffset);
				//���ŷ�Խ�Ķ���
				int len = FAnimations.ClimbAnims.HighClimb.Num();
				UAnimMontage* AnimMontage = FAnimations.ClimbAnims.HighClimb[UKismetMathLibrary::RandomInteger(len)];
				CharacterRef->PlayAnimMontage(AnimMontage, 1.0f, NAME_None);
			}
		}
	}
	else
	{
		bClimbing = false;
	}
}

void UXClimbComponent::FinishHighClimb(bool bshouldplaylandingAnimation, bool blandinganimtype)
{
	if (!CharacterRef || !ControllerRef || !CapsuleComponent || !CharacterMovementIns) return;
	if (bFence)
	{
		bFence = false;
		JumpImpactLanding(bshouldplaylandingAnimation, blandinganimtype);

		CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		CharacterMovementIns->SetMovementMode(EMovementMode::MOVE_Walking, 0);
		bClimbing = false;
		CheckYawRotation();
		CharacterRef->EnableInput(ControllerRef);
	}
	else
	{
		if (bshouldplaylandingAnimation)
		{
			JumpImpactLanding(bshouldplaylandingAnimation, blandinganimtype);
		}
		if (bThickLedge)
		{
			CharacterRef->EnableInput(ControllerRef);
			CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			CharacterMovementIns->SetMovementMode(EMovementMode::MOVE_Walking, 0);
			bClimbing = false;
			CheckYawRotation();
			CharacterRef->EnableInput(ControllerRef);
		}
		else
		{
			//�����ǰǽ���޷�վ��������������Ҫ�������µĶ���
			int len = FAnimations.VaultAnims.HighVault_JumpDown.Num();
			UAnimMontage* AnimMontage = FAnimations.VaultAnims.HighVault_JumpDown[UKismetMathLibrary::RandomInteger(len)];
			CharacterRef->PlayAnimMontage(AnimMontage, 1.0f, NAME_None);
			CharacterRef->GetWorldTimerManager().SetTimer(
				JumpDownDelayTimerHandle,
				this,
				&UXClimbComponent::JumpDownDelay, 
				2.7f, 
				false
			);
		}
	}
}

void UXClimbComponent::LowClimbs()
{
	ClimbingTypes = EClimbingTypes::ECT_LowClimb;
	LowClimAction();
}

void UXClimbComponent::LowClimAction()
{
	if (!CharacterRef || !ControllerRef || !CapsuleComponent || !CharacterMovementIns) return;
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CharacterMovementIns->SetMovementMode(EMovementMode::MOVE_Flying, 0);
	if (bCanVault)
	{
		if (bThickLedge)
		{
			if (MovingCheck())
			{
				SetPlayerLocationSmooth(RunLowClimbDistanceOffset, RunLowClimbHeightOffset);
				//���ŷ�Խ�Ķ���
				int len = FAnimations.ClimbAnims.RunLowClimb.Num();
				UAnimMontage* AnimMontage = FAnimations.ClimbAnims.RunLowClimb[UKismetMathLibrary::RandomInteger(len)];
				CharacterRef->PlayAnimMontage(AnimMontage, 1.0f, NAME_None);
			}
			else
			{
				SetPlayerLocationSmooth(LowClimbDistanceOffset, LowClimbHeightOffset);
				//���ŷ�Խ�Ķ���
				int len = FAnimations.ClimbAnims.LowClimb.Num();
				UAnimMontage* AnimMontage = FAnimations.ClimbAnims.LowClimb[UKismetMathLibrary::RandomInteger(len)];
				CharacterRef->PlayAnimMontage(AnimMontage, 1.2f, NAME_None);
			}
		}
		else
		{
			if (MovingCheck())
			{
				SetPlayerLocationSmooth(RunLowVaultDistanceOffset,RunLowVaultHeightOffset);
				//���ŷ�Խ�Ķ���
				int len = FAnimations.VaultAnims.RunLowVault.Num();
				UAnimMontage* AnimMontage = FAnimations.VaultAnims.RunLowVault[UKismetMathLibrary::RandomInteger(len)];
				CharacterRef->PlayAnimMontage(AnimMontage, 1.0f, NAME_None);
			}
			else
			{
				SetPlayerLocationSmooth(LowVaultDistanceOffset, LowVaultHeightOffset);
				//���ŷ�Խ�Ķ���
				int len = FAnimations.VaultAnims.LowVault.Num();
				UAnimMontage* AnimMontage = FAnimations.VaultAnims.LowVault[UKismetMathLibrary::RandomInteger(len)];
				CharacterRef->PlayAnimMontage(AnimMontage, 1.0f, NAME_None);
			}
		}
	}
	else
	{
		if (MovingCheck())
		{
			SetPlayerLocationSmooth(RunLowClimbDistanceOffset,RunLowClimbHeightOffset);
			//���ŷ�Խ�Ķ���
			int len = FAnimations.ClimbAnims.RunLowClimb.Num();
			UAnimMontage* AnimMontage = FAnimations.ClimbAnims.RunLowClimb[UKismetMathLibrary::RandomInteger(len)];
			CharacterRef->PlayAnimMontage(AnimMontage, 1.0f, NAME_None);
		}
		else
		{
			SetPlayerLocationSmooth(LowClimbDistanceOffset,LowClimbHeightOffset);
			//���ŷ�Խ�Ķ���
			int len = FAnimations.ClimbAnims.LowClimb.Num();
			UAnimMontage* AnimMontage = FAnimations.ClimbAnims.LowClimb[UKismetMathLibrary::RandomInteger(len)];
			CharacterRef->PlayAnimMontage(AnimMontage, 1.2f, NAME_None);
		}
	}
}

void UXClimbComponent::FinishLowClimb(bool bshouldplaylandingAnimation, bool blandinganimtype)
{
}

void UXClimbComponent::ClimbOrVault()
{
	if (bClimbing)
	{
		bVerifyClimbing = true;
		int check = TraceCaculation();
		if (check == 0)
		{
			WallThicknessCheck();
			bIsWallHit = false;
		}
		if (bVerifyClimbing)
		{
			//ִ�ж�Ӧ��Climb����
			if (bJumpClimb)
			{
				bJumpClimb = false;
				JumpClimbs();
			}
			else
			{
				if (bShouldClimb)
				{
					HighClimbs();
				}
				else
				{
					LowClimbs();
				}
			}
		}
	}
}

int UXClimbComponent::TraceCaculation()
{
	FHitResult OutHit;
	bool NoClimb = GetWallTrace(OutHit);
	//��������޷�վ������ôִ�з�Խ
	if (WalkAbleSurface(OutHit))
	{
		return -1;
	}
	bool HitWall = SetWallTrace(OutHit, NoClimb);
	if (!HitWall)
	{
		return -1;
	}
	bClimbing = true;
	CheckYawRotation();
	FHitResult HeightHit;
	bool Height = GetWallHeightTrace(HeightHit);
	SetWallHeightTrace(HeightHit, Height);
	//���ǽ�ڵĸ߶ȣ��ж�Ӧ��ִ��ʲô����
	WallHeightCheck();
	//�жϵ�ǰ��ǽ���ܷ�վ��
	if (GetFarWallHeight(FarWallHeight))
	{
		return 0;
	}
	else
	{
		FarWallHeight = { 0.0f,0.0f,0.0f };
		bThickLedge = false;
		if (LedgeCheck())
		{
			return 1;
		}
		else
		{
			if (FenceCheck())
			{
				return 1;
			}
		}
	}
	return -1;
}

void UXClimbComponent::FinishClimb(bool bshouldplaylandingAnimation, bool bvaultLanding)
{
	switch (ClimbingTypes)
	{
	case EClimbingTypes::ECT_None:
		break;
	case EClimbingTypes::ECT_JumpClimb:
		FinishJumpClimb(bshouldplaylandingAnimation, bvaultLanding);
		break;
	case EClimbingTypes::ECT_HighClimb:
		FinishHighClimb(bshouldplaylandingAnimation, bvaultLanding);
		break;
	case EClimbingTypes::ECT_LowClimb:
		if (bshouldplaylandingAnimation)
		{
			JumpImpactLanding(bshouldplaylandingAnimation, bvaultLanding);
		}
		CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		CharacterMovementIns->SetMovementMode(EMovementMode::MOVE_Walking, 0);
		bClimbing = false;
		CheckYawRotation();
		break;
	default:
		break;
	}
}


