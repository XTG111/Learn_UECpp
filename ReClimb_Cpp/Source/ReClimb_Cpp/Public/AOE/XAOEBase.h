// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "XAOEBase.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAOEOverlapActor, AActor*, actor);
UCLASS()
class RECLIMB_CPP_API AXAOEBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AXAOEBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	TArray<AActor*> SphereOverlapActors;
	TArray<AActor*> IgnoreActors;
	UPROPERTY(EditAnywhere)
		float SphereRadius = 200.0f;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = { EObjectTypeQuery::ObjectTypeQuery3 };

	virtual void Trigger();
	void CheckSphere();
	FOnAOEOverlapActor OnAOEOverlapActor;

};
