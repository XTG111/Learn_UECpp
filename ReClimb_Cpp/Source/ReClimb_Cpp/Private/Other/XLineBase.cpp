// Fill out your copyright notice in the Description page of Project Settings.


#include "Other/XLineBase.h"
#include "Components/SplineComponent.h"
#include "Components/SceneComponent.h"

// Sets default values
AXLineBase::AXLineBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PatrolRoute = CreateDefaultSubobject<USplineComponent>(TEXT("PatrolRoute"));
	PatrolRoute->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AXLineBase::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AXLineBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector AXLineBase::GetSplinePointAsWorldPosition()
{
	return PatrolRoute->GetLocationAtSplinePoint(PatrolIndex, ESplineCoordinateSpace::World);
}

void AXLineBase::IncrementPatrolRoute()
{
	UE_LOG(LogTemp, Warning, TEXT("PatrolIndex:%d"), PatrolIndex);
	if (PatrolIndex == (PatrolRoute->GetNumberOfSplinePoints() - 1))
	{
		Direction = -1;
	}
	else
	{
		if (PatrolIndex == 0)
		{
			Direction = 1;
		}
	}
	PatrolIndex += Direction;
}

