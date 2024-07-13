// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/XCombatComponent.h"
#include "Character/XClimbCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Projectiles/XProjectiles_Dark.h"
#include "DamageSystem/XDamageInterface.h"
#include "Perception/AIPerceptionSystem.h"
#include "Perception/AISense_Damage.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "AIInterface/XAIInterface.h"
#include "AOE/XAOEBase.h"
#include "AI/XAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "AI/XAI_Character.h"
#include "Component/XPlayerStatsComponent.h"

// Sets default values for this component's properties
UXCombatComponent::UXCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UXCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	CharacterEx = Cast<AXClimbCharacter>(GetOwner());
	CompOwner = GetOwner();
	//spin timeline
	SpinTimeline = NewObject<UTimelineComponent>(this, FName(TEXT("SpinTimeLine")));
	OnSpinTimelineTickCallBack.BindDynamic(this, &UXCombatComponent::SpinTimelineTickCall);
	SpinTimeline->AddInterpFloat(SpinFloatCurve, OnSpinTimelineTickCallBack);
	OnSpinTimelineFinishedCallBack.BindDynamic(this, &UXCombatComponent::SpinTimelineFinishedCall);
	SpinTimeline->SetTimelineFinishedFunc(OnSpinTimelineFinishedCallBack);
	SpinTimeline->RegisterComponent();

	//initial
	LastRelRotation = Cast<ACharacter>(GetOwner())->GetMesh()->GetRelativeRotation();
}


// Called every frame
void UXCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	SetHUDCrossHair(DeltaTime);
	// ...
}

void UXCombatComponent::SetHUDCrossHair(float delta)
{
	if (CharacterEx == nullptr || CharacterEx->Controller == nullptr) return;
	PIC = PIC == nullptr ? Cast<APlayerController>(CharacterEx->Controller) : PIC;
	if (PIC)
	{
		XCharacterHUD = XCharacterHUD == nullptr ? Cast<AXHUD>(PIC->GetHUD()) : XCharacterHUD;
		if (XCharacterHUD)
		{
			if (CharacterEx->GetPlayerStance() == EPlayerStance::EPS_Default)
			{
				HUDPackage.CrosshairsCenter = nullptr;
				HUDPackage.CrosshairsLeft = nullptr;
				HUDPackage.CrosshairsRight = nullptr;
				HUDPackage.CrosshairsTop = nullptr;
				HUDPackage.CrosshairsBottom = nullptr;
			}
			else if (CharacterEx->GetPlayerStance() == EPlayerStance::EPS_Magic)
			{
				HUDPackage.CrosshairsCenter = CrosshairCenter;
				HUDPackage.CrosshairsLeft = CrosshairLeft;
				HUDPackage.CrosshairsRight = CrosshairRight;
				HUDPackage.CrosshairsTop = CrosshairTop;
				HUDPackage.CrosshairsBottom = CrosshairBottom;
			}
			else return;
			HUDPackage.CrosshairColor = FColor::White;
			XCharacterHUD->SetHUDPackage(HUDPackage);
		}
	}
}

void UXCombatComponent::MagicSpell(FTransform& spawntransform, AActor* TargetActor, FDamageInfo damageinfo, TSubclassOf<AXProjectilesBase> spawnprojclass)
{
	if (!IsValid(spawnprojclass)) return;
	DamageInfo = damageinfo;
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = GetOwner();
	AXProjectilesBase* SpawnActor = GetWorld()->SpawnActor<AXProjectilesBase>(spawnprojclass, spawntransform, SpawnParameters);
	SpawnActor->TargetActor = TargetActor;
	SpawnActor->Speed = 1000.0f;
	SpawnActor->SphereCollision->IgnoreActorWhenMoving(GetOwner(), true);
	SpawnActor->OnProjectileImpact.AddDynamic(this, &UXCombatComponent::OnProjectileHit);
}

void UXCombatComponent::OnProjectileHit(AActor* OtherActor, const FHitResult& HitRes)
{
	if (OtherActor && OtherActor->Implements<UXDamageInterface>())
	{
		IXDamageInterface::Execute_TakeDamage(OtherActor, DamageInfo, GetOwner());
	}

	//对应蓝图中的ReportDamageEvent
	FAIDamageEvent DamageEvent;

	DamageEvent.Instigator = GetOwner();
	DamageEvent.Amount = DamageInfo.Amount;
	DamageEvent.Location = GetOwner()->GetActorLocation();
	DamageEvent.DamagedActor = OtherActor;

	UAIPerceptionSystem::GetCurrent(GetWorld())->OnEvent(DamageEvent);

	OnAttackEnded.Broadcast();
}

void UXCombatComponent::FireBullet(FVector TraceStart, FVector TraceEnd, FDamageInfo damageinfo, AActor* IgnoreActor)
{
	ETraceTypeQuery ETType = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1);
	TArray<FHitResult> HitResults;
	bool bIsHit = UKismetSystemLibrary::LineTraceMulti(
		GetWorld(),
		TraceStart,
		TraceEnd,
		ETType,
		false,
		TArray<AActor*>{IgnoreActor},
		EDrawDebugTrace::ForDuration,
		HitResults,
		true
	);

	if (bIsHit)
	{
		AActor* DamageActor = DamageFirstNonTeamActor(HitResults);
		if (DamageActor && DamageActor->Implements<UXDamageInterface>())
		{
			IXDamageInterface::Execute_TakeDamage(DamageActor, damageinfo, GetOwner());
		}

		FAIDamageEvent DamageEvent;

		DamageEvent.Instigator = GetOwner();
		DamageEvent.Amount = DamageInfo.Amount;
		DamageEvent.Location = GetOwner()->GetActorLocation();
		DamageEvent.DamagedActor = DamageActor;

		UAIPerceptionSystem::GetCurrent(GetWorld())->OnEvent(DamageEvent);
	}
}

void UXCombatComponent::SwordAttack(FVector TraceStart, FVector TraceEnd, FDamageInfo damageinfo, AActor* IgnoreActor)
{
	ETraceTypeQuery ETType = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1);
	TArray<FHitResult> HitResults;
	bool bIsHit = UKismetSystemLibrary::SphereTraceMulti(
		GetWorld(),
		TraceStart,
		TraceEnd,
		SwordRadius,
		ETType,
		false,
		TArray<AActor*>{IgnoreActor},
		EDrawDebugTrace::None,
		HitResults,
		true
	);

	if (bIsHit)
	{
		TArray<AActor*> DamageActors = DamageAllNonTeamActor(HitResults);
		for (auto& DamageActor : DamageActors)
		{
			if (DamageActor && DamageActor->Implements<UXDamageInterface>())
			{
				IXDamageInterface::Execute_TakeDamage(DamageActor, damageinfo, GetOwner());
			}

			FAIDamageEvent DamageEvent;

			DamageEvent.Instigator = GetOwner();
			DamageEvent.Amount = DamageInfo.Amount;
			DamageEvent.Location = GetOwner()->GetActorLocation();
			DamageEvent.DamagedActor = DamageActor;

			UAIPerceptionSystem::GetCurrent(GetWorld())->OnEvent(DamageEvent);
		}
	}
}

TArray<AActor*> UXCombatComponent::DamageAllNonTeamActor(TArray<FHitResult>& HitResults)
{
	TArray<AActor*> Res;
	int DamageActorNum = 0;
	int SelfActorNum = 0;
	for (auto& HitRes : HitResults)
	{
		AActor* DamageActor = Cast<AActor>(HitRes.Actor);
		if (DamageActor && DamageActor->Implements<UXDamageInterface>())
		{
			DamageActorNum = IXDamageInterface::Execute_GetTeamNumber(DamageActor);
		}
		if (GetOwner() && GetOwner()->Implements<UXDamageInterface>())
		{
			SelfActorNum = IXDamageInterface::Execute_GetTeamNumber(GetOwner());
		}
		if (DamageActorNum != SelfActorNum)
		{
			Res.AddUnique(DamageActor);
		}
	}
	return Res;
}

AActor* UXCombatComponent::DamageFirstNonTeamActor(TArray<FHitResult>& HitResults)
{
	int DamageActorNum = 0;
	int SelfActorNum = 0;
	for (auto& HitRes : HitResults)
	{
		AActor* DamageActor = Cast<AActor>(HitRes.Actor);
		if (DamageActor && DamageActor->Implements<UXDamageInterface>())
		{
			DamageActorNum = IXDamageInterface::Execute_GetTeamNumber(DamageActor);
		}
		if (GetOwner() && GetOwner()->Implements<UXDamageInterface>())
		{
			SelfActorNum = IXDamageInterface::Execute_GetTeamNumber(GetOwner());
		}
		if (DamageActorNum != SelfActorNum)
		{
			return DamageActor;
			break;
		}
	}
	return nullptr;
}

void UXCombatComponent::OnNotifyMontage(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (NotifyName == FName(TEXT("Smash")))
	{
		SmashAOE(SmashRadius);
	}
	if (NotifyName == FName(TEXT("Fire")))
	{
		SwordPrimaryDamage();
	}
	if (NotifyName == FName(TEXT("Jump")))
	{
		//Move Capulse to Play Animation
		JumpPrimaryDamage();
	}
	if (NotifyName == FName(TEXT("Spin")))
	{
		SpinMesh();
	}
	if (NotifyName == FName(TEXT("Chase")))
	{
		ChaseAttackTarget();
	}
	if (NotifyName == FName(TEXT("SpinAOE")))
	{
		SpinAOE(SpinRadius);
	}
	if (NotifyName == FName(TEXT("MageFire")))
	{
		MageBaseDamage();
	}
	if (NotifyName == FName(TEXT("MageBarrage")))
	{
		MageBarrageDamage();
	}
}

void UXCombatComponent::OnAttackMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	if (CompOwner && CompOwner->Implements<UXAIInterface>())
	{
		IXAIInterface::Execute_AttackEnd(CompOwner, AttackInfo.AttackTarget);
		//当被打断或则完成停止旋转，并恢复网格朝向
		SpinTimeline->Stop();
		Cast<ACharacter>(CompOwner)->GetMesh()->SetRelativeRotation(LastRelRotation);
	}
	if (CompOwner && CompOwner->Implements<UXDamageInterface>())
	{
		IXDamageInterface::Execute_SetIsInteruptible(CompOwner, true);
	}
}

void UXCombatComponent::GroundSmash(FAttackInfo& attackinfo, float smashradius)
{
	if (CompOwner && CompOwner->Implements<UXAIInterface>())
	{
		AttackInfo = attackinfo;
		SmashRadius = smashradius;
		IXAIInterface::Execute_Attack(CompOwner, attackinfo.AttackTarget);

		Cast<ACharacter>(CompOwner)->GetMesh()->GetAnimInstance()->OnMontageEnded.Clear();
		Cast<ACharacter>(CompOwner)->GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.Clear();
		Cast<ACharacter>(CompOwner)->GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &UXCombatComponent::OnNotifyMontage);
		Cast<ACharacter>(CompOwner)->GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &UXCombatComponent::OnAttackMontageEnd);
		Cast<ACharacter>(CompOwner)->PlayAnimMontage(attackinfo.AttackMontage);

		if (CompOwner && CompOwner->Implements<UXDamageInterface>())
		{
			IXDamageInterface::Execute_SetIsInteruptible(CompOwner, false);
		}
	}
}

void UXCombatComponent::SmashAOE(float radius)
{
	FTransform SpawnTrans = FTransform();
	SpawnTrans.SetLocation(CompOwner->GetActorLocation());
	SmashAOEActor = GetWorld()->SpawnActor<AXAOEBase>(SmashAOEClass, SpawnTrans);
	SmashAOEActor->SphereRadius = radius;
	SmashAOEActor->OnAOEOverlapActor.AddDynamic(this, &UXCombatComponent::AOEDamageForOverlapActor);
	SmashAOEActor->Trigger();
}

void UXCombatComponent::SpinAOE(float radius)
{
	FTransform SpawnTrans = FTransform();
	SpawnTrans.SetLocation(CompOwner->GetActorLocation());
	SpinAOEActor = GetWorld()->SpawnActor<AXAOEBase>(SpinAOEClass, SpawnTrans);
	SpinAOEActor->SphereRadius = radius;
	SpinAOEActor->OnAOEOverlapActor.AddDynamic(this, &UXCombatComponent::AOEDamageForOverlapActor);
	SpinAOEActor->Trigger();
}

void UXCombatComponent::AOEDamageForOverlapActor(AActor* actor)
{
	if (actor != CompOwner && actor->Implements<UXDamageInterface>())
	{
		IXDamageInterface::Execute_TakeDamage(actor, AttackInfo.DamageInfo, CompOwner);
	}
}

void UXCombatComponent::SwordPrimaryAttack(FAttackInfo& attackinfo, float length, float radius)
{
	if (CompOwner && CompOwner->Implements<UXAIInterface>())
	{
		AttackInfo = attackinfo;
		SwordLength = length;
		SwordRadius = radius;
		IXAIInterface::Execute_Attack(CompOwner, attackinfo.AttackTarget);

		Cast<ACharacter>(CompOwner)->GetMesh()->GetAnimInstance()->OnMontageEnded.Clear();
		Cast<ACharacter>(CompOwner)->GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.Clear();
		Cast<ACharacter>(CompOwner)->GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &UXCombatComponent::OnNotifyMontage);
		Cast<ACharacter>(CompOwner)->GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &UXCombatComponent::OnAttackMontageEnd);
		Cast<ACharacter>(CompOwner)->PlayAnimMontage(attackinfo.AttackMontage);

		if (CompOwner && CompOwner->Implements<UXDamageInterface>())
		{
			IXDamageInterface::Execute_SetIsInteruptible(CompOwner, false);
		}
	}
}

void UXCombatComponent::SwordPrimaryDamage()
{
	FVector Start = CompOwner->GetActorLocation();

	AXAIController* AIC = Cast<AXAIController>(CompOwner->GetInstigatorController());

	if (AIC)
	{
		if (AttackInfo.AttackTarget && AttackInfo.AttackTarget->Implements<UXDamageInterface>())
		{
			if (!IXDamageInterface::Execute_IsDead(AttackInfo.AttackTarget))
			{
				FVector End = Start + CompOwner->GetActorForwardVector() * SwordLength;
				SwordAttack(Start, End, AttackInfo.DamageInfo, CompOwner);
			}
			else AIC->SetStateAsPassive();
		}
	}
}

void UXCombatComponent::JumpPrimaryAttack(FAttackInfo& attackinfo)
{
	if (CompOwner && CompOwner->Implements<UXAIInterface>())
	{
		AttackInfo = attackinfo;
		IXAIInterface::Execute_Attack(CompOwner, attackinfo.AttackTarget);

		Cast<ACharacter>(CompOwner)->GetMesh()->GetAnimInstance()->OnMontageEnded.Clear();
		Cast<ACharacter>(CompOwner)->GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.Clear();
		Cast<ACharacter>(CompOwner)->GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &UXCombatComponent::OnNotifyMontage);
		Cast<ACharacter>(CompOwner)->GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &UXCombatComponent::OnAttackMontageEnd);
		Cast<ACharacter>(CompOwner)->PlayAnimMontage(attackinfo.AttackMontage);

		if (CompOwner && CompOwner->Implements<UXDamageInterface>())
		{
			IXDamageInterface::Execute_SetIsInteruptible(CompOwner, false);
		}
	}
}

FVector UXCombatComponent::PredicPlayerLoc(AActor* player, float pretime)
{
	//获取pretime后player的位置
	FVector res = player->GetVelocity() * FVector { 1.0f, 1.0f, 0.0f };//clear z
	res *= pretime;
	res += player->GetActorLocation();
	return res;
}

void UXCombatComponent::JumpPrimaryDamage()
{
	FVector LaunchVelocity;
	FVector Location = PredicPlayerLoc(AttackInfo.AttackTarget);
	FVector EndLoc = FVector{ Location.X,Location.Y,Location.Z };
	UGameplayStatics::SuggestProjectileVelocity_CustomArc(
		GetWorld(),
		LaunchVelocity,
		CompOwner->GetActorLocation(),
		EndLoc,
		0.0f,
		0.5f
	);
	Cast<ACharacter>(CompOwner)->LandedDelegate.Clear();
	Cast<ACharacter>(CompOwner)->LaunchCharacter(LaunchVelocity, true, true);
	Cast<ACharacter>(CompOwner)->LandedDelegate.AddDynamic(this, &UXCombatComponent::OnLand);
}

void UXCombatComponent::OnLand(const FHitResult& Hit)
{
	Cast<ACharacter>(CompOwner)->LandedDelegate.Clear();
	Cast<ACharacter>(CompOwner)->GetCharacterMovement()->StopMovementImmediately();
}

void UXCombatComponent::SpinMesh()
{
	LastRelRotation = Cast<ACharacter>(CompOwner)->GetMesh()->GetRelativeRotation();
	SpinTimeline->Play();
}

void UXCombatComponent::SpinTimelineTickCall(float value)
{
	FRotator NewRot = FRotator{
	LastRelRotation.Roll,
	UKismetMathLibrary::Lerp(LastRelRotation.Yaw, LastRelRotation.Yaw + 360.0f * 12.0f, value),
	LastRelRotation.Pitch
	//UKismetMathLibrary::Lerp(LastRelRotation.Yaw, 0.0f, value)//
	};
	//GetMesh()->SetRelativeRotation(NewRot);
	Cast<ACharacter>(CompOwner)->GetMesh()->SetRelativeRotation(NewRot);
}

void UXCombatComponent::SpinTimelineFinishedCall()
{
	CompOwner->GetWorldTimerManager().ClearTimer(ChaseLoopTimer);
}

void UXCombatComponent::ChaseAttackTarget()
{
	Cast<AXAI_Character>(CompOwner)->AIController->ReceiveMoveCompleted.Clear();
	Cast<AXAI_Character>(CompOwner)->AIController->ReceiveMoveCompleted.AddDynamic(this, &UXCombatComponent::MoveEndCall);
	Cast<AXAI_Character>(CompOwner)->AIController->MoveToActor(AttackInfo.AttackTarget, 200.0f, false);
}

void UXCombatComponent::MoveEndCall(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	if (Result == EPathFollowingResult::Success)
	{
		if (Cast<AXAI_Character>(CompOwner)->bAttacking)
		{
			CompOwner->GetWorldTimerManager().SetTimer(ChaseLoopTimer, this, &UXCombatComponent::ChaseAttackTarget, 0.1f);
		}
	}
}

void UXCombatComponent::SpinAttack(FAttackInfo& attackinfo, float radius)
{
	if (CompOwner && CompOwner->Implements<UXAIInterface>())
	{
		AttackInfo = attackinfo;
		SpinRadius = radius;
		IXAIInterface::Execute_Attack(CompOwner, attackinfo.AttackTarget);

		Cast<ACharacter>(CompOwner)->GetMesh()->GetAnimInstance()->OnMontageEnded.Clear();
		Cast<ACharacter>(CompOwner)->GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.Clear();
		Cast<ACharacter>(CompOwner)->GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &UXCombatComponent::OnNotifyMontage);
		Cast<ACharacter>(CompOwner)->GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &UXCombatComponent::OnAttackMontageEnd);
		Cast<ACharacter>(CompOwner)->PlayAnimMontage(attackinfo.AttackMontage);

		if (CompOwner && CompOwner->Implements<UXDamageInterface>())
		{
			IXDamageInterface::Execute_SetIsInteruptible(CompOwner, false);
		}
	}
}

void UXCombatComponent::MageBaseAttack(FAttackInfo& attackinfo, TSubclassOf<AXProjectilesBase> attackprojectileEx)
{
	if (CompOwner && CompOwner->Implements<UXAIInterface>())
	{
		AttackInfo = attackinfo;
		AttackProjectileEx = attackprojectileEx;
		IXAIInterface::Execute_Attack(CompOwner, attackinfo.AttackTarget);

		Cast<ACharacter>(CompOwner)->GetMesh()->GetAnimInstance()->OnMontageEnded.Clear();
		Cast<ACharacter>(CompOwner)->GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.Clear();
		Cast<ACharacter>(CompOwner)->GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &UXCombatComponent::OnNotifyMontage);
		Cast<ACharacter>(CompOwner)->GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &UXCombatComponent::OnAttackMontageEnd);
		Cast<ACharacter>(CompOwner)->PlayAnimMontage(attackinfo.AttackMontage);

		if (CompOwner && CompOwner->Implements<UXDamageInterface>())
		{
			IXDamageInterface::Execute_SetIsInteruptible(CompOwner, false);
		}
	}
}

void UXCombatComponent::MageBaseDamage()
{
	FTransform trans;
	trans.SetLocation(Cast<ACharacter>(CompOwner)->GetMesh()->GetSocketLocation(TEXT("hand_rsocket")));
	FVector TargetLoc = AttackInfo.AttackTarget->GetActorLocation();

	FRotator Rot = UKismetMathLibrary::FindLookAtRotation(
		Cast<ACharacter>(CompOwner)->GetMesh()->GetSocketLocation(TEXT("hand_rSocket")),
		TargetLoc
	);
	trans.SetRotation(Rot.Quaternion());
	MagicSpell(trans, AttackInfo.AttackTarget, AttackInfo.DamageInfo, AttackProjectileEx);
}

void UXCombatComponent::MageBarrageAttack(FAttackInfo& attackinfo, TSubclassOf<AXProjectilesBase> normalprojectileEx)
{
	if (CompOwner && CompOwner->Implements<UXAIInterface>())
	{
		AttackInfo = attackinfo;
		NormalProjectileEx = normalprojectileEx;
		IXAIInterface::Execute_Attack(CompOwner, attackinfo.AttackTarget);

		Cast<ACharacter>(CompOwner)->GetMesh()->GetAnimInstance()->OnMontageEnded.Clear();
		Cast<ACharacter>(CompOwner)->GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.Clear();
		Cast<ACharacter>(CompOwner)->GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &UXCombatComponent::OnNotifyMontage);
		Cast<ACharacter>(CompOwner)->GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &UXCombatComponent::OnAttackMontageEnd);
		Cast<ACharacter>(CompOwner)->PlayAnimMontage(attackinfo.AttackMontage);

		if (CompOwner && CompOwner->Implements<UXDamageInterface>())
		{
			IXDamageInterface::Execute_SetIsInteruptible(CompOwner, false);
		}
	}
}

void UXCombatComponent::MageBarrageDamage()
{
	FTransform trans;
	trans.SetLocation(Cast<ACharacter>(CompOwner)->GetMesh()->GetSocketLocation(TEXT("hand_rsocket")));

	FVector TargetLoc = AttackInfo.AttackTarget->GetActorLocation();

	FRotator Rot = UKismetMathLibrary::FindLookAtRotation(
		Cast<ACharacter>(CompOwner)->GetMesh()->GetSocketLocation(TEXT("hand_rSocket")),
		FVector{ TargetLoc.X,TargetLoc.Y + UKismetMathLibrary::RandomFloatInRange(-300.0f,300.0f) ,TargetLoc.Z }
	);
	trans.SetRotation(Rot.Quaternion());

	MagicSpell(trans, AttackInfo.AttackTarget, AttackInfo.DamageInfo, NormalProjectileEx);
	if (CompOwner && CompOwner->Implements<UXDamageInterface>())
	{
		IXDamageInterface::Execute_SetIsInteruptible(CompOwner, true);
	}
}

