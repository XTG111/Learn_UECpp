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

void AXAI_SwordCharacter::Attack_Implementation(AActor* AttakTarget)
{
	//UE_LOG(LogTemp, Warning, TEXT("Attack"));
	ShortAttack(AttakTarget);
}

void AXAI_SwordCharacter::GetIdealRange_Implementation(float& AttackRadius, float& DefendRadius)
{
	AttackRadius = 200.0f;
	DefendRadius = 1200.0f;
}

void AXAI_SwordCharacter::StartBlock()
{
	AIStatesComponent->SetIsBlocking(true);
	BlockStace = EBlockingStace::EBS_Blocking;
	//当阻挡时立即停止移动
	GetCharacterMovement()->StopMovementImmediately();
	AIStatesComponent->OnBlocked.Broadcast(true);
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

void AXAI_SwordCharacter::CallOnBlocked(bool bCanbeParried)
{
	//Super::CallOnBlocked(bCanbeParried);
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

void AXAI_SwordCharacter::EndAttackMontage(UAnimMontage* Montage, bool bInterrupted)
{
	UE_LOG(LogTemp, Warning, TEXT("EndAttack"));
	IXAIInterface::Execute_AttackEnd(this, AttackTargetActor);
	AIStatesComponent->SetbIsInterruptible(true);
}

void AXAI_SwordCharacter::OnNotifyMontage(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (NotifyName == FName(TEXT("Fire")))
	{
		CauseDamage();
	}
	if (NotifyName == FName(TEXT("Jump")))
	{
		//Move Capulse to Play Animation
		JumpToAttackTarget(AttackTargetActor);
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
	GetMesh()->GetAnimInstance()->OnMontageEnded.Clear();
	GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.Clear();
	GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &AXAI_SwordCharacter::OnNotifyMontage);
	GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &AXAI_SwordCharacter::EndAttackMontage);
	PlayAnimMontage(AttackMontage);

	AIStatesComponent->SetbIsInterruptible(false);
}

void AXAI_SwordCharacter::CauseDamage()
{
	FVector Start = GetActorLocation();

	AXAIController* AIC = Cast<AXAIController>(GetController());

	FDamageInfo DamageInfo;
	DamageInfo.Amount = 25.0f;
	DamageInfo.DamageType = EDamageType::EDT_Melee;
	DamageInfo.DamageResponse = EDamageResponse::EDR_HitReaction;

	if (AIC)
	{
		if (AttackTargetActor && AttackTargetActor->Implements<UXDamageInterface>())
		{
			if (!IXDamageInterface::Execute_IsDead(AttackTargetActor))
			{
				FVector End = Start + GetActorForwardVector() * 300.0f;
				CombatComponent->SwordAttack(Start, End, DamageInfo, this);
			}
			else AIC->SetStateAsPassive();
		}
	}
}

void AXAI_SwordCharacter::JumpAttack(AActor* AttakTarget)
{
	Super::Attack_Implementation(AttakTarget);
	GetMesh()->GetAnimInstance()->OnMontageEnded.Clear();
	GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.Clear();
	GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &AXAI_SwordCharacter::OnNotifyMontage);
	GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &AXAI_SwordCharacter::EndAttackMontage);
	PlayAnimMontage(JumpAttackMontage);

	AIStatesComponent->SetbIsInterruptible(false);
}

void AXAI_SwordCharacter::JumpToAttackTarget(AActor* AttakTarget)
{
	FVector LaunchVelocity;
	FVector Location = PredicPlayerLoc(AttakTarget);
	FVector EndLoc = FVector{ Location.X,Location.Y,Location.Z };
	UGameplayStatics::SuggestProjectileVelocity_CustomArc(
		GetWorld(),
		LaunchVelocity,
		GetActorLocation(),
		EndLoc,
		0.0f,
		0.5f
	);
	LandedDelegate.Clear();
	LaunchCharacter(LaunchVelocity, true, true);
	LandedDelegate.AddDynamic(this, &AXAI_SwordCharacter::OnLand);
}

void AXAI_SwordCharacter::OnLand(const FHitResult& Hit)
{
	//落地时立即停止运动，这样可以播放本身的落地动画
	LandedDelegate.Clear();
	GetCharacterMovement()->StopMovementImmediately();
}

FVector AXAI_SwordCharacter::PredicPlayerLoc(AActor* player, float pretime)
{
	//获取pretime后player的位置
	FVector res = player->GetVelocity() * FVector { 1.0f, 1.0f, 0.0f };//clear z
	res *= pretime;
	res += player->GetActorLocation();
	return res;
}
