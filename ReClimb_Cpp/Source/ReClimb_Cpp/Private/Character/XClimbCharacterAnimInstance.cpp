// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/XClimbCharacterAnimInstance.h"
#include "Character/XClimbCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UXClimbCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	XClimbCharacter = Cast<AXClimbCharacter>(TryGetPawnOwner());
}

void UXClimbCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	if (!IsValid(XClimbCharacter)) return;
	bIsInAir = XClimbCharacter->GetCharacterMovement()->IsFalling();
	speed = XClimbCharacter->GetVelocity().Size();
	Direction = CalculateDirection(XClimbCharacter->GetVelocity(), XClimbCharacter->GetActorRotation());
}
