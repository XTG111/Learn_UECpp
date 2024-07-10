// Fill out your copyright notice in the Description page of Project Settings.


#include "AOE/XAOE_Heal.h"
#include "Kismet/GameplayStatics.h"

void AXAOE_Heal::Trigger()
{
	Super::Trigger();
	HealEffect = UGameplayStatics::SpawnEmitterAttached(
		AttachParticle,
		GetRootComponent(),
		NAME_None,
		FVector{ 0.0f,0.0f,0.0f },
		FRotator{ 0.0f,0.0f,0.0f },
		EAttachLocation::KeepRelativeOffset
	);
}
