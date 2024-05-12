// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/EndVaultOrClimbNotify.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "Character/XClimbCharacter.h"
#include "Component/XClimbComponent.h"

bool UEndVaultOrClimbNotify::Received_Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) const
{
    
    if (IsValid(MeshComp->GetOwner()))
    {
        UE_LOG(LogTemp, Warning, TEXT("MeshComp->GetOwner()FinishedNify!"));
        AXClimbCharacter* CharacterRef = Cast<AXClimbCharacter>(MeshComp->GetOwner());
        UXClimbComponent* ClimbComponent = Cast<UXClimbComponent>(CharacterRef->GetComponentByClass(UXClimbComponent::StaticClass()));

        if(IsValid(ClimbComponent))
        {
            UE_LOG(LogTemp, Warning, TEXT("ClimbComponentFinishedNify!"));
            ClimbComponent->FinishClimb(bShouldPlayLandingAnim, bLandingTypeToggle);
        }
    }
    return true;
}
