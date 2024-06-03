// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/XSwordBase.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"

// Sets default values
AXSwordBase::AXSwordBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = RootSceneComponent;

	SwordMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwordMesh"));
	SwordMeshComp->SetupAttachment(RootComponent);

	SwordMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	StaticMeshPath = TEXT("StaticMesh'/Game/Weapon_Pack/Mesh/Weapons/Weapons_Kit/SM_Sword.SM_Sword'");

}

// Called when the game starts or when spawned
void AXSwordBase::BeginPlay()
{
	Super::BeginPlay();
	UStaticMesh* StaticMesh = LoadObject<UStaticMesh>(nullptr, *StaticMeshPath);
	if (SwordMeshComp && StaticMesh)
	{
		// 将静态网格资源分配给静态网格组件
		SwordMeshComp->SetStaticMesh(StaticMesh);
	}
}

// Called every frame
void AXSwordBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

