// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/XClimbCharacterAnimInstance.h"
#include "Character/XClimbCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Component/XClimbComponent.h"

void UXClimbCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	XClimbCharacter = Cast<AXClimbCharacter>(TryGetPawnOwner());
}

void UXClimbCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	if (!IsValid(XClimbCharacter)) return;
	Stance = XClimbCharacter->GetPlayerStance();
	bIsInAir = XClimbCharacter->GetCharacterMovement()->IsFalling();
	bInAttack = XClimbCharacter->GetInAttack();
	speed = XClimbCharacter->GetVelocity().Size();
	Direction = CalculateDirection(XClimbCharacter->GetVelocity(), XClimbCharacter->GetActorRotation());
	FootIK();
}

void UXClimbCharacterAnimInstance::FootIK()
{
	if (!XClimbCharacter) return;
	UXClimbComponent* XClimbComp = Cast<UXClimbComponent>(XClimbCharacter->GetComponentByClass(UXClimbComponent::StaticClass()));
	if (!XClimbComp) return;
	LeftFootLocationIK.X = XClimbComp->GetIKLeftFootOffset();
	RightFootLocationIK.X = XClimbComp->GetIKRightFootOffset() * (-1.0f);
	CaftLeftLocationIK = XClimbComp->GetCaftLeftLocation();
	CaftRightLocationIK = XClimbComp->GetCaftRightLocation();
	HipOffsetIK = XClimbComp->GetIKHipOffset();
	LeftFootRotationIK = XClimbComp->GetIKLeftFootRotation();
	RightFootRotationIK = XClimbComp->GetIKRightFootRotation();

	LeftHandLocationIK = XClimbComp->GetLeftHandLocation();
	RightHandLocationIK = XClimbComp->GetRightHandLocation();
	bClimbing = XClimbComp->GetbClimbing();

}
