// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/XClimbComponent.h"

// Sets default values for this component's properties
UXClimbComponent::UXClimbComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UXClimbComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UXClimbComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UXClimbComponent::InitCharacter(ACharacter* Player, UArrowComponent* WallTraceArrow, UCapsuleComponent* Capsule, UCharacterMovementComponent* CharacterMovement)
{
}

void UXClimbComponent::StopMontage()
{
}

void UXClimbComponent::SetFPMesh(USkeletalMeshComponent* FPMesh)
{
}

bool UXClimbComponent::ClimbingCheck()
{
	return bClimbing;
}

