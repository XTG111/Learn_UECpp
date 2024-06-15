// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/XAI_GunCharacter.h"
#include "Weapon/XSwordBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AI/XAIController.h"
#include "Component/XCombatComponent.h"

void AXAI_GunCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (Implements<UXAIInterface>())
	{
		Execute_EquipWeapon(this);
	}

}

void AXAI_GunCharacter::EquipWeapon_Implementation()
{
	//UE_LOG(LogTemp, Warning, TEXT("Rifle_EquipWeapon"));
	SetIsWiledWeapon(true);
	if (Sword)
	{
		//UE_LOG(LogTemp, Warning, TEXT("RifleSword"));
		Weapon = GetWorld()->SpawnActor<AXSwordBase>(Sword->GetDefaultObject()->GetClass(), GetActorTransform());

		FAttachmentTransformRules AttachmentRules(
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			true
		);
		if (Weapon)
		{
			UE_LOG(LogTemp, Warning, TEXT("RifleWeapon"));
			Weapon->AttachToComponent(
				GetMesh(),
				AttachmentRules,
				FName(TEXT("GunSocketR"))
			);
		}
	}
	CallOnEquipWeapon.Broadcast();
}

void AXAI_GunCharacter::UnEquipWeapon_Implementation()
{
}

float AXAI_GunCharacter::SetMovementSpeed_Implementation(EAIMovement SpeedEnum)
{
	float res = 0.0f;
	switch (SpeedEnum)
	{
	case EAIMovement::EAM_Idle:
		GetCharacterMovement()->MaxWalkSpeed = 0.0f;
		res = 0.0f;
		break;
	case EAIMovement::EAM_Walking:
		GetCharacterMovement()->MaxWalkSpeed = 250.0f;
		res = 250.0f;
		break;
	case EAIMovement::EAM_Joggiing:
		GetCharacterMovement()->MaxWalkSpeed = 325.0f;
		res = 325.0f;
		break;
	case EAIMovement::EAM_Sprinting:
		GetCharacterMovement()->MaxWalkSpeed = 500.0f;
		res = 500.0f;
		break;
	default:
		break;
	}
	return res;
}

void AXAI_GunCharacter::GetIdealRange_Implementation(float& AttackRadius, float& DefendRadius)
{
	AttackRadius = 600.0f;
	DefendRadius = 600.0f;
}

void AXAI_GunCharacter::Attack_Implementation()
{
	GetMesh()->GetAnimInstance()->OnMontageEnded.Clear();
	GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.Clear();
	PlayAnimMontage(AttackMontage);
	GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &AXAI_GunCharacter::OnNotifyMontage);
	GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &AXAI_GunCharacter::OnAttackMontageEnd);
}

void AXAI_GunCharacter::OnNotifyMontage(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (NotifyName == FName(TEXT("Fire")))
	{
		FVector Start = Weapon->GetActorLocation();

		AXAIController* AIC = Cast<AXAIController>(GetController());

		FDamageInfo DamageInfo;
		DamageInfo.Amount = 10.0f;
		DamageInfo.DamageType = EDamageType::EDT_Projectile;
		DamageInfo.DamageResponse = EDamageResponse::EDR_HitReaction;
		DamageInfo.bCanBeBlocked = true;

		if (AIC)
		{
			{
				AActor* TargetActor = AIC->GetAttackTargetActor();
				if (TargetActor && TargetActor->Implements<UXDamageInterface>())
				{
					if (!IXDamageInterface::Execute_IsDead(TargetActor))
					{
						FVector End = TargetActor->GetActorLocation();
						CombatComponent->FireBullet(Start, End, DamageInfo, this);
					}
					else AIC->SetStateAsPassive();
				}
			}
		}
	}
}

void AXAI_GunCharacter::OnAttackMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	CallOnAttackEndCall.Broadcast();
}

