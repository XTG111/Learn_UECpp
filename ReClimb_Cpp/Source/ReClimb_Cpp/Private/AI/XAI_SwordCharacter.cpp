// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/XAI_SwordCharacter.h"
#include "Weapon/XSwordBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "AI/XAIController.h"
#include "Component/XCombatComponent.h"
#include "Component/XPlayerStatsComponent.h"
#include "Components/WidgetComponent.h"
#include "Widget/XWidget_EnemyHeadHP.h"
#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"
#include "AOE/XAOEBase.h"

void AXAI_SwordCharacter::BeginPlay()
{
	Super::BeginPlay();
	AIStatesComponent->OnBlocked.Clear();
	AIStatesComponent->OnDamageResponse.Clear();
	AIStatesComponent->OnBlocked.AddDynamic(this, &AXAI_SwordCharacter::CallOnBlocked);
	AIStatesComponent->OnDamageResponse.AddDynamic(this, &AXAI_SwordCharacter::CallOnDamageResponse);
}

void AXAI_SwordCharacter::EquipWeapon_Implementation()
{
	WieldSword();
}

void AXAI_SwordCharacter::UnEquipWeapon_Implementation()
{
	SheathSword();
}

void AXAI_SwordCharacter::GetIdealRange_Implementation(float& AttackRadius, float& DefendRadius)
{
	AttackRadius = 200.0f;
	DefendRadius = 400.0f;
}

void AXAI_SwordCharacter::StartBlock()
{
	AIStatesComponent->SetIsBlocking(true);
	BlockStace = EBlockingStace::EBS_Blocking;
	//当阻挡时立即停止移动
	GetCharacterMovement()->StopMovementImmediately();
	AIStatesComponent->OnBlocked.Broadcast(AttackTargetActor, true);
	GetWorldTimerManager().ClearTimer(BlockEndTimer);
	GetWorldTimerManager().SetTimer(BlockEndTimer, this, &AXAI_SwordCharacter::EndBlock, 2.0f);

}

void AXAI_SwordCharacter::EndBlockMontage(UAnimMontage* Montage, bool bInterrupted)
{
	BlockStace = EBlockingStace::EBS_None;
	AIStatesComponent->SetIsBlocking(false);
	//if interuppted Attack
	CallOnAttackEndCall.Broadcast();
	OnBlockEnded.Broadcast();
}

void AXAI_SwordCharacter::EndBlock()
{
	BlockStace = EBlockingStace::EBS_None;
	AIStatesComponent->SetIsBlocking(false);
	//if interuppted Attack
	CallOnAttackEndCall.Broadcast();
	OnBlockEnded.Broadcast();
}

void AXAI_SwordCharacter::CallOnBlocked(AActor* DamageCauser, bool bCanbeParried)
{
	//Super::CallOnBlocked(DamageCauser, bCanbeParried);
	//IXDamageInterface::Execute_ReturnAttackToken(DamageCauser, CurAttackNeedToken);
	BlockStace = EBlockingStace::EBS_BlockSuccess;
	GetWorldTimerManager().ClearTimer(BlockEndTimer);
	GetMesh()->GetAnimInstance()->OnMontageEnded.Clear();
	PlayAnimMontage(Block2Montage);
	GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &AXAI_SwordCharacter::EndBlockMontage);
}

void AXAI_SwordCharacter::CallOnDamageResponse(EDamageResponse DamageResponse, AActor* DamageCausor)
{
	if (bCanBlock)
	{
		StartBlock();
		bCanBlock = false;
		GetWorldTimerManager().SetTimer(CoolDownBlock, this, &AXAI_SwordCharacter::SetbCanBlock, 10.0f, false);
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("SwordDamage"));
		Super::CallOnDamageResponse(DamageResponse, DamageCausor);
	}
}

void AXAI_SwordCharacter::WieldSword()
{
	bIsWiledWeapon = true;
	GetMesh()->GetAnimInstance()->OnMontageEnded.Clear();
	PlayAnimMontage(SwordBegin);
	GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &AXAI_SwordCharacter::EndWieldMontage);

	if (Sword)
	{
		UE_LOG(LogTemp, Warning, TEXT("Sword"));
		Weapon = GetWorld()->SpawnActor<AXSwordBase>(Sword->GetDefaultObject()->GetClass(), GetActorTransform());

		FAttachmentTransformRules AttachmentRules(
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			true
		);
		if (Weapon)
		{
			UE_LOG(LogTemp, Warning, TEXT("Weapon"));
			Weapon->AttachToComponent(
				GetMesh(),
				AttachmentRules,
				FName(TEXT("WeaponHandR"))
			);
		}
	}
}

void AXAI_SwordCharacter::EndWieldMontage(UAnimMontage* Montage, bool bInterrupted)
{
	CallOnEquipWeapon.Broadcast();
}

void AXAI_SwordCharacter::SheathSword()
{
	GetMesh()->GetAnimInstance()->OnMontageEnded.Clear();
	PlayAnimMontage(SwordEnd);
	GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &AXAI_SwordCharacter::EndSheathMontage);
	bIsWiledWeapon = false;
}

void AXAI_SwordCharacter::EndSheathMontage(UAnimMontage* Montage, bool bInterrupted)
{
	if (Weapon)	Weapon->Destroy();
	CallOnUnEquipWeapon.Broadcast();
}

bool AXAI_SwordCharacter::TakeDamage_Implementation(FDamageInfo DamageInfo, AActor* DamageCausor)
{
	if (DamageInfo.bCanBeBlocked)
	{
		TryToBlock();
		return false;
	}
	return AIStatesComponent->TakeDamage(DamageInfo, DamageCausor);
}

void AXAI_SwordCharacter::TryToBlock()
{
	int temp = UKismetMathLibrary::RandomFloat();
	if (temp >= BlockProbability)
	{
		StartBlock();
	}
}

void AXAI_SwordCharacter::SetbCanBlock()
{
	bCanBlock = true;
	GetWorldTimerManager().ClearTimer(CoolDownBlock);
}

void AXAI_SwordCharacter::ShortAttack(AActor* AttakTarget)
{
	Super::Attack_Implementation(AttakTarget);
	FDamageInfo DamageInfo;
	DamageInfo.Amount = 10.0f;
	DamageInfo.DamageType = EDamageType::EDT_Projectile;
	DamageInfo.DamageResponse = EDamageResponse::EDR_HitReaction;
	DamageInfo.bCanBeBlocked = true;
	DamageInfo.bCanBeParried = true;

	FAttackInfo AttackInfo;
	AttackInfo.AttackMontage = AttackMontage;
	AttackInfo.AttackTarget = AttackTargetActor;
	AttackInfo.DamageInfo = DamageInfo;
	CombatComponent->SwordPrimaryAttack(AttackInfo, 300.0f, 20.0f);
}

void AXAI_SwordCharacter::JumpAttack(AActor* AttakTarget)
{
	Super::Attack_Implementation(AttakTarget);
	FDamageInfo DamageInfo;
	DamageInfo.Amount = 15.0f;
	DamageInfo.DamageType = EDamageType::EDT_Projectile;
	DamageInfo.DamageResponse = EDamageResponse::EDR_HitReaction;
	DamageInfo.bCanBeBlocked = true;
	DamageInfo.bCanBeParried = true;

	FAttackInfo AttackInfo;
	AttackInfo.AttackMontage = JumpAttackMontage;
	AttackInfo.AttackTarget = AttackTargetActor;
	AttackInfo.DamageInfo = DamageInfo;
	CombatComponent->JumpPrimaryAttack(AttackInfo);
}

void AXAI_SwordCharacter::SpinAttack(AActor* AttakTarget)
{
	Super::Attack_Implementation(AttakTarget);
	FDamageInfo DamageInfo;
	DamageInfo.Amount = 5.0f;
	DamageInfo.DamageType = EDamageType::EDT_Projectile;
	DamageInfo.DamageResponse = EDamageResponse::EDR_HitReaction;
	DamageInfo.bCanBeBlocked = true;

	FAttackInfo AttackInfo;
	AttackInfo.AttackMontage = SpinAttackMontage;
	AttackInfo.AttackTarget = AttackTargetActor;
	AttackInfo.DamageInfo = DamageInfo;
	CombatComponent->SpinAttack(AttackInfo, 200.f);
}

void AXAI_SwordCharacter::SmashAttack(AActor* AttakTarget)
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

