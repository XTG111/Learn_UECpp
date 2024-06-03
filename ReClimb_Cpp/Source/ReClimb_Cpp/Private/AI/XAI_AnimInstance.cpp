// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/XAI_AnimInstance.h"
#include "AI/XAI_Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AI/XAIController.h"

void UXAI_AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	XAINPC = Cast<AXAI_Character>(UGameplayStatics::GetActorOfClass(GetWorld(), AXAI_Character::StaticClass()));
}

void UXAI_AnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	if (!IsValid(XAINPC)) return;
	bIsInAir = XAINPC->GetCharacterMovement()->IsFalling();
	speed = XAINPC->GetVelocity().Size();
	Direction = CalculateDirection(XAINPC->GetVelocity(), XAINPC->GetActorRotation());

	if (IsValid(XAINPC->GetController()))
	{
		AXAIController* AIC = Cast<AXAIController>(XAINPC->GetController());
		if (IsValid(AIC->GetFocusActor()))
		{
			bIsFocusingTarget = true;
		}
		else
		{
			bIsFocusingTarget = false;
		}
	}
}
