// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/StopMontageNotify.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"

void UStopMontageNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    Super::Notify(MeshComp, Animation);
    UAnimMontage* AnimMontage = Cast<UAnimMontage>(Animation);
    UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
    AnimInstance->Montage_Stop(AnimMontage->GetDefaultBlendOutTime(), AnimMontage);
}
