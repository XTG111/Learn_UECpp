// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/StopMontageNotify.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"

bool UStopMontageNotify::Received_Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) const
{
    UAnimMontage* AnimMontage = Cast<UAnimMontage>(Animation);
    UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
    AnimInstance->Montage_Stop(AnimMontage->GetDefaultBlendOutTime(), AnimMontage);
    return true;
}
