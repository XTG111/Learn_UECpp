// Fill out your copyright notice in the Description page of Project Settings.


#include "AOE/XAOEBase.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AXAOEBase::AXAOEBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AXAOEBase::BeginPlay()
{
	Super::BeginPlay();
	//Trigger();
}

// Called every frame
void AXAOEBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AXAOEBase::Trigger()
{
	CheckSphere();
}

void AXAOEBase::CheckSphere()
{
	//检测圆形范围内是否有Pawn
	UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		GetActorLocation(),
		SphereRadius,
		ObjectTypes,
		nullptr,
		IgnoreActors,
		SphereOverlapActors
	);
	for (auto& actor : SphereOverlapActors)
	{
		OnAOEOverlapActor.Broadcast(actor);
	}
}

