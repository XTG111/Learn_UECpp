// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/XAI_MageCharacter.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Component/XCombatComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/XAIController.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "AOE/XAOE_Heal.h"
#include "Component/XPlayerStatsComponent.h"
#include "Projectiles/XProjectilesBase.h"


AXAI_MageCharacter::AXAI_MageCharacter()
{
}

float AXAI_MageCharacter::SetMovementSpeed_Implementation(EAIMovement SpeedEnum)
{
	switch (SpeedEnum)
	{
	case EAIMovement::EAM_Idle:
		return 0.0f;
		break;
	case EAIMovement::EAM_Walking:
		return 200.0f;
		break;
	case EAIMovement::EAM_Joggiing:
		return 350.0f;
		break;
	case EAIMovement::EAM_Sprinting:
		return 500.0f;
		break;
	default:
		break;
	}
	return 0.0f;
}

void AXAI_MageCharacter::GetIdealRange_Implementation(float& AttackRadius, float& DefendRadius)
{
	AttackRadius = 500.0f;
	DefendRadius = 700.0f;
}

void AXAI_MageCharacter::Teleport(FVector& TargetLoc)
{
	if (bInTeleport)
	{
		OnTeleportEnd.Broadcast();
		return;
	}
	bInTeleport = true;
	//设置移动类型为Fly
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	GetCharacterMovement()->MaxFlySpeed = 5000.0f;
	GetCharacterMovement()->MaxAcceleration = 99999.0f;
	//隐藏网格体
	GetMesh()->SetVisibility(false, true);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	//视觉效果
	TeleportTrailEffect = UGameplayStatics::SpawnEmitterAttached(
		MoveParticle,
		GetMesh(),
		FName(TEXT("Spine1")),
		FVector{ 0.0f,0.0f,0.0f },
		FRotator{ 0.0f,0.0f,0.0f },
		EAttachLocation::KeepRelativeOffset
	);
	TeleportBodyEffect = UGameplayStatics::SpawnEmitterAttached(
		AttachParticle,
		GetMesh(),
		FName(TEXT("Spine1")),
		FVector{ 0.0f,0.0f,0.0f },
		FRotator{ 0.0f,0.0f,0.0f },
		EAttachLocation::KeepRelativeOffset
	);

	AXAIController* AICon = Cast<AXAIController>(GetController());
	AICon->ReceiveMoveCompleted.Clear();
	AICon->ReceiveMoveCompleted.AddDynamic(this, &AXAI_MageCharacter::MoveEndCall);
	StopAnimMontage();
	AICon->MoveToLocation(TargetLoc, 15.0f, false);
}

void AXAI_MageCharacter::MoveEndCall(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	//设置移动类型为Fly
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	GetCharacterMovement()->MaxAcceleration = 1500.0f;
	//隐藏网格体
	GetMesh()->SetVisibility(true, true);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	OnTeleportEnd.Broadcast();
	GetWorldTimerManager().SetTimer(ParticleDestroyTimer, this, &AXAI_MageCharacter::DestroyParticle, 0.5f, false);
}

void AXAI_MageCharacter::DestroyParticle()
{
	bInTeleport = false;
	if (TeleportBodyEffect)
	{
		TeleportBodyEffect->DestroyComponent();
	}
	if (TeleportTrailEffect)
	{
		TeleportTrailEffect->DestroyComponent();
	}
	GetWorldTimerManager().ClearTimer(ParticleDestroyTimer);
}

void AXAI_MageCharacter::HealOverTime()
{
	GetMesh()->GetAnimInstance()->OnMontageEnded.Clear();
	GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &AXAI_MageCharacter::HealEnd);
	PlayAnimMontage(HealAnimation);
	FTransform SpawnTrans = FTransform();
	SpawnTrans.SetLocation(GetActorLocation());
	HealAOEActor = GetWorld()->SpawnActor<AXAOE_Heal>(HealAOEClass, SpawnTrans);
	HealAOEActor->SpawnInterval = 1.0f;
	HealAOEActor->Duration = 10.0f;
	HealAOEActor->Trigger();
	HealAOEActor->OnAOEOverlapActor.AddDynamic(this, &AXAI_MageCharacter::HealActor);
}

void AXAI_MageCharacter::HealEnd(UAnimMontage* Montage, bool bInterrupted)
{
	if (HealAOEActor) HealAOEActor->Destroy();
	OnHealOverTimeEnd.Broadcast();
}

void AXAI_MageCharacter::HealActor(AActor* actor)
{
	if (actor == this && actor->Implements<UXDamageInterface>())
	{
		IXDamageInterface::Execute_Heal(actor, AIStatesComponent->GetMaxHealth() * 0.05f);
	}

}

void AXAI_MageCharacter::BaseAttack(AActor* AttakTarget)
{
	Super::Attack_Implementation(AttakTarget);
	FDamageInfo DamageInfo;
	DamageInfo.Amount = 20.0f;
	DamageInfo.DamageType = EDamageType::EDT_Projectile;
	DamageInfo.DamageResponse = EDamageResponse::EDR_HitReaction;
	DamageInfo.bCanBeBlocked = true;

	FAttackInfo AttackInfo;
	AttackInfo.AttackMontage = AttackMontage;
	AttackInfo.AttackTarget = AttackTargetActor;
	AttackInfo.DamageInfo = DamageInfo;

	CombatComponent->MageBaseAttack(AttackInfo, AttackProjectileEx);

}

void AXAI_MageCharacter::BarrageAttack(AActor* AttakTarget)
{
	Super::Attack_Implementation(AttakTarget);
	FDamageInfo DamageInfo;
	DamageInfo.Amount = 5.0f;
	DamageInfo.DamageType = EDamageType::EDT_Projectile;
	DamageInfo.DamageResponse = EDamageResponse::EDR_HitReaction;
	DamageInfo.bCanBeBlocked = true;

	FAttackInfo AttackInfo;
	AttackInfo.AttackMontage = NormalAttackAnimation;
	AttackInfo.AttackTarget = AttackTargetActor;
	AttackInfo.DamageInfo = DamageInfo;

	CombatComponent->MageBarrageAttack(AttackInfo, NormalProjectileEx);
}

void AXAI_MageCharacter::SmashAttack(AActor* AttakTarget)
{
	Super::Attack_Implementation(AttakTarget);
	FDamageInfo DamageInfo;
	DamageInfo.Amount = 25.0f;
	DamageInfo.DamageType = EDamageType::EDT_Projectile;
	DamageInfo.DamageResponse = EDamageResponse::EDR_HitReaction;
	DamageInfo.bCanBeBlocked = true;

	FAttackInfo AttackInfo;
	AttackInfo.AttackMontage = SmashMontage;
	AttackInfo.AttackTarget = AttackTargetActor;
	AttackInfo.DamageInfo = DamageInfo;
	CombatComponent->GroundSmash(AttackInfo, 300.0f);
}

