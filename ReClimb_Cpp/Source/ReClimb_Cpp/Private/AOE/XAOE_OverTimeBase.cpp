// Fill out your copyright notice in the Description page of Project Settings.


#include "AOE/XAOE_OverTimeBase.h"

void AXAOE_OverTimeBase::Trigger()
{
	Super::Trigger();
	GetWorldTimerManager().SetTimer(SpawnLoopTimer, this, &AXAOE_OverTimeBase::SpawnEmitter, SpawnInterval, true);
	GetWorldTimerManager().SetTimer(DestroyTimer, this, &AXAOE_OverTimeBase::DestroyEmitter, Duration, false);
}

void AXAOE_OverTimeBase::SpawnEmitter()
{
	CheckSphere();
}

void AXAOE_OverTimeBase::DestroyEmitter()
{
	GetWorldTimerManager().ClearTimer(SpawnLoopTimer);
	GetWorldTimerManager().ClearTimer(DestroyTimer);
	//Ïú»Ù
	Destroy();
}
