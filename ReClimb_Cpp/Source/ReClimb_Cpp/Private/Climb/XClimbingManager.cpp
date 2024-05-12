// Fill out your copyright notice in the Description page of Project Settings.


#include "Climb/XClimbingManager.h"
#include "Character/XClimbCharacter.h"
#include "Component/XClimbComponent.h"


// Sets default values
AXClimbingManager::AXClimbingManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AXClimbingManager::BeginPlay()
{
	Super::BeginPlay();

	if (CWFloatCurve)
	{
		CWTimeline = NewObject<UTimelineComponent>(this, "CWTimeline");
		OnCWTimeLineTickCallBack.BindDynamic(this, &AXClimbingManager::CWTimelineTickCallBack);

		CWTimeline->AddInterpFloat(CWFloatCurve, OnCWTimeLineTickCallBack);
		//当时间轴事件结束的委托
		OnCWTimelineFinishedCallBack.BindDynamic(this, &AXClimbingManager::CWTimelineFinishedCallBack);
		CWTimeline->SetTimelineFinishedFunc(OnCWTimelineFinishedCallBack);

		//注册组件
		CWTimeline->RegisterComponent();
	}

}

// Called every frame
void AXClimbingManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CWTimeline->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, NULL);
}

void AXClimbingManager::CWTimelineTickCallBack(float value)
{
	if (!XClimbCharacer) return;
	Time = value;
	UXClimbComponent* ClimbComp = Cast<UXClimbComponent>(XClimbCharacer->GetComponentByClass(UXClimbComponent::StaticClass()));
	if (!ClimbComp) return;
 	float Alpha = InterpolateTimeByHeight(Time, ClimbComp);
	if (Alpha <= 0.0f) return;
	ClimbComp->SetPlayerLocation(PlayerDistanceOffset, HeightOffset, Alpha);
	ClimbComp->FaceWall(Time);
}

void AXClimbingManager::CWTimelineFinishedCallBack()
{
	CWTimeline->Stop();
}

void AXClimbingManager::SetPlayerLocationTimeline(float playerdistanceoffset, const FVector& heightoffset, AXClimbCharacter* CharRef)
{
	if (!CharRef) return;
	XClimbCharacer = CharRef;
	PlayerDistanceOffset = playerdistanceoffset;
	HeightOffset = heightoffset;

	CWTimeline->PlayFromStart();
}

float AXClimbingManager::InterpolateTimeByHeight(float time, UXClimbComponent* ComponentRef)
{
	if (!ComponentRef) return -1.0f;
	float res = (ComponentRef->GetWallHeight() - ComponentRef->GetWallLocation()).Z;
	res /= ComponentRef->GetInterpolationSpeed();
	res = time / res;
	res /= 2;
	return res;
}

