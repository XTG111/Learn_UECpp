// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/XAI_SwordCharacter.h"
#include "Weapon/XSwordBase.h"

void AXAI_SwordCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AXAI_SwordCharacter::EquipWeapon_Implementation()
{
	WieldSword();
}

void AXAI_SwordCharacter::UnEquipWeapon_Implementation()
{
	SheathSword();
}

void AXAI_SwordCharacter::Attack_Implementation()
{
	GetMesh()->GetAnimInstance()->OnMontageEnded.Clear();
	PlayAnimMontage(AttackMontage);
	GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &AXAI_SwordCharacter::EndAttackMontage);
}

void AXAI_SwordCharacter::EndAttackMontage(UAnimMontage* Montage, bool bInterrupted)
{
	CallOnAttackEndCall.Broadcast();
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
