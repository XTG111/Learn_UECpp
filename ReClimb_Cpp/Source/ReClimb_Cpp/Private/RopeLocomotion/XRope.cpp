// Fill out your copyright notice in the Description page of Project Settings.


#include "RopeLocomotion/XRope.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AXRope::AXRope()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	RootComponent = SkeletalMeshComponent;

}

// Called when the game starts or when spawned
void AXRope::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AXRope::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

